-- ============================================================
-- 患者表插入 650000 条测试数据（15万 + 50万）
-- 用于测试 MVC 与非 MVC 模式的性能差异
-- ============================================================

USE hospital_db;

-- 可选：查看当前患者数量
SELECT COUNT(*) AS patients_before FROM patients;

DELIMITER //

DROP PROCEDURE IF EXISTS fill_patients//

-- start_idx: 起始编号（用于 name/phone/email 等唯一标识）
-- cnt: 插入条数
CREATE PROCEDURE fill_patients(IN start_idx INT, IN cnt INT)
BEGIN
    DECLARE i INT DEFAULT start_idx;
    DECLARE end_idx INT DEFAULT start_idx + cnt - 1;
    DECLARE batch_size INT DEFAULT 500;
    DECLARE batch_count INT DEFAULT 0;
    DECLARE batch_sql LONGTEXT DEFAULT '';
    DECLARE first_in_batch BOOLEAN DEFAULT TRUE;
    DECLARE pname VARCHAR(100);
    DECLARE pphone VARCHAR(20);
    DECLARE pemail VARCHAR(100);
    DECLARE paddress VARCHAR(200);
    DECLARE pbirth DATE;
    DECLARE pgender VARCHAR(10);
    DECLARE phistory VARCHAR(200);

    -- 开始事务，加速批量插入
    START TRANSACTION;

    WHILE i <= end_idx DO
        SET batch_sql = 'INSERT INTO patients (name, phone, email, address, birth_date, gender, medical_history) VALUES ';
        SET first_in_batch = TRUE;
        SET batch_count = 0;

        -- 每批插入 batch_size 条
        WHILE batch_count < batch_size AND i <= end_idx DO
            SET pname = CONCAT('患者_', LPAD(i, 7, '0'));
            SET pphone = CONCAT('138', LPAD((i MOD 100000000), 8, '0'));
            SET pemail = CONCAT('patient_', i, '@test.hospital.com');
            SET paddress = CONCAT('测试地址', (i MOD 1000), '号');
            SET pbirth = DATE_SUB(CURDATE(), INTERVAL (20 + (i MOD 60)) YEAR);
            SET pgender = IF(i MOD 2 = 0, 'Male', 'Female');
            SET phistory = ELT(1 + (i MOD 5), '无', '高血压', '糖尿病', '哮喘', '心脏病');

            IF NOT first_in_batch THEN
                SET batch_sql = CONCAT(batch_sql, ',');
            END IF;
            SET batch_sql = CONCAT(batch_sql, '(',
                QUOTE(pname), ',',
                QUOTE(pphone), ',',
                QUOTE(pemail), ',',
                QUOTE(paddress), ',',
                QUOTE(DATE_FORMAT(pbirth, '%Y-%m-%d')), ',',
                QUOTE(pgender), ',',
                QUOTE(phistory),
            ')');
            SET first_in_batch = FALSE;
            SET batch_count = batch_count + 1;
            SET i = i + 1;
        END WHILE;

        SET @stmt = batch_sql;
        PREPARE prep FROM @stmt;
        EXECUTE prep;
        DEALLOCATE PREPARE prep;
    END WHILE;

    COMMIT;
END//

DELIMITER ;

-- 插入 150000 条患者
CALL fill_patients(1, 150000);
-- 再插入 500000 条患者（若表中已有 15 万条，可只执行此行：CALL fill_patients(150001, 500000);）
CALL fill_patients(150001, 500000);

DROP PROCEDURE IF EXISTS fill_patients;

-- 验证
SELECT COUNT(*) AS patients_after FROM patients;
SELECT MIN(id) AS min_id, MAX(id) AS max_id FROM patients;
