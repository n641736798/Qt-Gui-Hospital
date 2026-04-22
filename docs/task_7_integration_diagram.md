# Task 7 Integration Architecture

## System Integration Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                         MainWindow                              │
│                                                                 │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐         │
│  │   Patients   │  │   Medical    │  │ Appointments │         │
│  │     Page     │  │  Records Page│  │     Page     │         │
│  └──────────────┘  └──────────────┘  └──────────────┘         │
│         │                  │                  │                 │
│         └──────────────────┴──────────────────┘                │
│                            │                                    │
│                    ┌───────▼────────┐                          │
│                    │  DatabaseManager│                          │
│                    └───────┬────────┘                          │
└────────────────────────────┼─────────────────────────────────┘
                             │
                    ┌────────▼────────┐
                    │  MySQL Database │
                    │                 │
                    │  ┌───────────┐  │
                    │  │ patients  │  │
                    │  └─────┬─────┘  │
                    │        │         │
                    │        │ ON DELETE CASCADE
                    │        │         │
                    │  ┌─────▼─────┐  │
                    │  │  medical  │  │
                    │  │  records  │  │
                    │  └───────────┘  │
                    └─────────────────┘
```

## Data Synchronization Flow

### Patient Update Flow:
```
User Updates Patient
        │
        ▼
MainWindow::onEditPatientClicked()
        │
        ▼
DatabaseManager::updatePatient()
        │
        ├─► Update database
        │
        └─► emit patientDataChanged()
                │
                ▼
        MedicalRecordWidget::refreshData()
                │
                ▼
        Medical records display updated patient name
```

### Patient Delete Flow (with Cascade):
```
User Deletes Patient
        │
        ▼
MainWindow::onDeletePatientClicked()
        │
        ▼
DatabaseManager::deletePatient()
        │
        ├─► DELETE FROM patients WHERE id = ?
        │   │
        │   └─► MySQL CASCADE DELETE
        │       │
        │       └─► Automatically deletes medical_records
        │
        └─► emit patientDeleted(patientId)
            │
            └─► emit patientDataChanged()
                    │
                    ▼
            MedicalRecordWidget::refreshData()
                    │
                    ▼
            Medical records list refreshed (deleted records removed)
```

## Signal-Slot Connections

```cpp
// In MainWindow::setupMedicalRecordsPage()

DatabaseManager                    MedicalRecordWidget
     │                                    │
     │  patientDataChanged()              │
     ├───────────────────────────────────►│ refreshData()
     │                                    │
     │  patientDeleted(int)               │
     ├───────────────────────────────────►│ refreshData()
     │                                    │
```

## Database Schema with Cascade Delete

```sql
CREATE TABLE patients (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(20),
    email VARCHAR(100),
    -- ... other fields
);

CREATE TABLE medical_records (
    id INT AUTO_INCREMENT PRIMARY KEY,
    patient_id INT NOT NULL,
    diagnosis TEXT NOT NULL,
    -- ... other fields
    
    -- CASCADE DELETE: When patient is deleted, 
    -- all their medical records are automatically deleted
    FOREIGN KEY (patient_id) 
        REFERENCES patients(id) 
        ON DELETE CASCADE
);
```

## Key Implementation Details

### 1. Cascade Delete (Database Level)
- **Location**: `src/databasemanager.cpp` - `createMedicalRecordTable()`
- **Mechanism**: MySQL foreign key constraint with `ON DELETE CASCADE`
- **Benefit**: Automatic, atomic, and guaranteed by database engine

### 2. Data Synchronization (Application Level)
- **Location**: `include/databasemanager.h` - Signal declarations
- **Mechanism**: Qt signal-slot pattern
- **Signals**:
  - `patientDataChanged()`: Emitted on patient update or delete
  - `patientDeleted(int patientId)`: Emitted on patient delete
- **Benefit**: Loose coupling, automatic UI updates

### 3. Deleted Patient Marker (Safety Feature)
- **Location**: `src/databasemanager.cpp` - `getAllMedicalRecords()`
- **Mechanism**: LEFT JOIN with null check
- **Fallback**: Shows "患者已删除" for orphaned records
- **Benefit**: Graceful handling of edge cases

## Testing Strategy

### Unit Tests
1. **testCascadeDeleteOnPatientDeletion()**
   - Creates patient with multiple medical records
   - Deletes patient
   - Verifies all medical records are automatically deleted

2. **testPatientInfoSynchronization()**
   - Creates patient and medical record
   - Updates patient name
   - Verifies medical record shows updated name

3. **testDeletedPatientMarker()**
   - Verifies orphaned records show "患者已删除"
   - Tests LEFT JOIN behavior

### Integration Testing
- Manual testing of UI synchronization
- Verify medical records page refreshes when:
  - Patient is updated in patients page
  - Patient is deleted in patients page
- Verify cascade delete works in production database

## Requirements Mapping

| Requirement | Implementation | Status |
|------------|----------------|--------|
| 1.3 - Cascade delete | Foreign key ON DELETE CASCADE | ✅ |
| 2.1 - Main window integration | setupMedicalRecordsPage() | ✅ |
| 2.2 - Tab display | on_medicalRecordsButton_clicked() | ✅ |
| 9.4 - Patient info sync | Signal-slot mechanism | ✅ |
| 9.5 - Deleted patient marker | LEFT JOIN with null check | ✅ |

## Conclusion

Task 7 successfully integrates the medical records management feature into the main application with:
- ✅ Proper tab navigation
- ✅ Database-level cascade delete
- ✅ Application-level data synchronization
- ✅ Graceful handling of deleted patients
- ✅ Comprehensive test coverage

The implementation follows Qt and database best practices, ensuring data integrity and a responsive user interface.
