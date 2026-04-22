-- ============================================================
-- 病历表插入 10000 条测试数据
-- 从 patients 表按顺序取真实 id 作为 patient_id（支持 id 不连续）
-- ============================================================

USE hospital_db;

-- 可选：查看当前病历数量
SELECT COUNT(*) AS medical_records_before FROM medical_records;

-- 可选：确认患者数量与 ID 范围
SELECT COUNT(*) AS patient_count, MIN(id) AS min_id, MAX(id) AS max_id FROM patients;

DELIMITER //

DROP PROCEDURE IF EXISTS fill_medical_records//

CREATE PROCEDURE fill_medical_records(IN cnt INT)
BEGIN
    DECLARE i INT DEFAULT 1;
    DECLARE pid INT DEFAULT NULL;
    DECLARE no_more INT DEFAULT 0;

    DECLARE cur CURSOR FOR SELECT id FROM patients ORDER BY id;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET no_more = 1;

    -- 无患者则报错
    IF (SELECT COUNT(*) FROM patients) = 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'patients 表为空，请先执行患者表插入脚本';
    END IF;

    OPEN cur;

    WHILE i <= cnt DO
        FETCH cur INTO pid;
        IF no_more THEN
            SET no_more = 0;
            CLOSE cur;
            OPEN cur;
            FETCH cur INTO pid;
        END IF;

        INSERT INTO medical_records
            (patient_id, created_date, diagnosis, symptoms, treatment, doctor_name, notes)
        VALUES
            (
                pid,
                DATE_SUB(NOW(), INTERVAL (i MOD 365) DAY),
                CONCAT('诊断记录 #', i),
                CONCAT('症状描述 #', i),
                CONCAT('治疗方案 #', i),
                CONCAT('医生', ((i - 1) MOD 20) + 1),
                CONCAT('备注 #', i)
            );
        SET i = i + 1;
    END WHILE;

    CLOSE cur;
END//

DELIMITER ;

-- 插入 10000 条病历
CALL fill_medical_records(10000);

DROP PROCEDURE IF EXISTS fill_medical_records;

-- 验证
SELECT COUNT(*) AS medical_records_after FROM medical_records;
