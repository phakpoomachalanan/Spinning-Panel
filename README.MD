# โครงงานวิศวกรรม
ภาควิชาวิศวกรรมคอมพิวเตอร์  คณะวิศวกรรมศาสตร์ มหาวิทยาลัยเกษตรศาสตร์


# ชื่อโครงงาน
**Spinning Panel**

# รายชื่อ
1. ปัญญวัฒน์ ครองกิติชู
เลขประจำตัว
6410504144


2. ภาคภูมิ อจละนันท์
เลขประจำตัว
6410504225


3. สิรภัทร สิริภัทรวณิช
เลขประจำตัว
6410504373


4. ภาสวิชญ์ ลอยมา
เลขประจำตัว
6510503689


5. ศุภกฤต สงสุวงค์
เลขประจำตัว
6510503808


# ที่มาและความสำคัญ
เนื่องด้วยการใช้งาน solar panel ที่เพิ่มขึ้นเป็นอย่างมากในปัจจุบันจึงทำให้อยากทำแบบจำลองการหมุนตามแสงของ solar panel เพื่อทำให้สามารถรับแสงได้ดีที่สุดในทุกเวลาและสามารถให้ผู้ใช้ตรวจสอบผ่านทั้งเว็บไซต์และจอแสดงผลได้


# ความสามารถ
หมุนแผ่นรับแสงตามทิศของแสง
แสดงผลไฟที่ผลิตได้ตอนนั้น ผลิตได้เท่าไหร่แล้ว ลดค่าไฟได้เท่าไหร่แล้ว
ทำ Interface ด้วย node-red


# อุปกรณ์ที่ใช้
- ES P32 5 ตัว
- LDR 4 ตัว
- OLED Display 64*32 1 จอ
- Servo 2 ตัว (180°)
- Breadboard
- สายไฟ


# ตารางการทำงานของบอร์ด

| เลขบอร์ด | MAC Address        | การทำงาน                                          |
|-----------|--------------------|-----------------------------------------------------|
| 1         | 3C:61:05:03:41:A0 | - ใช้ ESP-NOW ในการติดต่อระหว่างบอร์ด             |
|           |                    | - คำนวณทิศทางการหมุน                             |
|           |                    | - รับค่าแสงจากบอร์ด                                |
| 2         | 3C:61:05:03:D5:CC | - ส่งทิศทางการหมุนไปให้บอร์ด 3                   |
|           |                    | - ส่งค่าแสงไปให้บอร์ด 4                           |
|           |                    | - ส่งค่าแสงและทิศทางการหมุนไปให้บอร์ด 5        |
| 3         | E8:DB:84:00:FB:C4 | - รับค่าแสงจาก LDR และส่งค่าแสงให้บอร์ด 1         |
| 4         | 24:6F:28:28:99:9C | - รับค่าทิศทางการหมุนจากบอร์ด 1 และคุมการหมุนของ Servo ทั้งสองตัว |
|           |                    | - รับข้อมูลค่าแสงจากบอร์ด 1 และโชว์ค่าสถานะบนจอด้วย I2C |
| 5         | E8:68:E7:22:B0:E4 | - รับข้อมูลค่าแสง, ทิศทางการหมุนของ Servo และควบคุม Interface ด้วย Node-Red |


# อุปกรณ์ที่ต่อกับบอร์ด
<!-- # ตารางการทำงานและอุปกรณ์ที่เชื่อมต่อกับแต่ละบอร์ด -->
| เลขบอร์ด | MAC Address        | อุปกรณ์                                            |
|-----------|--------------------|-----------------------------------------------------|
| 1         | 3C:61:05:03:41:A0 | -                                                     |
| 2         | 3C:61:05:03:D5:CC | LDR 4 ตัว                                           |
| 3         | E8:DB:84:00:FB:C4 | Servo 180° 2 ตัว                                   |
| 4         | 24:6F:28:28:99:9C | OLED Display 1 จอ, Button 1 ปุ่ม                   |
| 5         | E8:68:E7:22:B0:E4 | -                                                     |

<!-- 8. แผนภาพการเชื่อมต่อ

Physical

Logical

**** -->