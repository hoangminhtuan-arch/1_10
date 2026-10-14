# BÁO CÁO PHÂN TÍCH LỖI VÀ BẢNG TEST CASES
## Bài tập 01: Dò vết Lỗi Truyền Tham trị (Pass-by-value) Khi Cập nhật Giá Vé Sự kiện

---

### 1. Phân tích lỗi mã nguồn & Nguyên nhân kỹ thuật

#### 1.1. Sự cố 1: Lỗi truyền tham trị (Pass-by-value) làm mất trạng thái biến `soLuong`
- **Hiện tượng**: Khách hàng đặt vé và nhận thông báo đặt thành công, nhưng khi hiển thị giỏ hàng trong `main()`, hệ thống báo *"Hiện chưa có vé nào được đặt thành công trong hệ thống!"* (biến `soLuong` trong `main()` luôn bằng 0).
- **Vị trí dòng lệnh sai**:
  - Khai báo nguyên mẫu hàm: 
    ```c
    void datVeConcert(struct VeBan dsVe[], int soLuong, struct VeBan veMoi);
    ```
  - Lệnh tăng số lượng bên trong hàm `datVeConcert`:
    ```c
    soLuong++; // Chỉ tăng biến cục bộ trong stack frame của hàm
    ```
  - Lệnh gọi hàm trong `main()`:
    ```c
    datVeConcert(dsVe, soLuong, ve1);
    datVeConcert(dsVe, soLuong, ve2);
    ```
- **Nguyên nhân kỹ thuật**:
  - Trong ngôn ngữ lập trình C, các kiểu dữ liệu cơ bản (primitive types như `int`) khi truyền vào hàm mặc định sử dụng cơ chế **truyền tham trị (pass-by-value)**.
  - Khi `main()` gọi `datVeConcert(dsVe, soLuong, ve1);`, một bản sao độc lập của giá trị `soLuong` (bằng 0) được đẩy vào ngăn xếp (stack frame) của hàm `datVeConcert`.
  - Phép toán `soLuong++` chỉ làm tăng bản sao cục bộ đó lên 1. Khi hàm kết thúc và trả về điều khiển cho `main()`, stack frame bị hủy, giá trị của biến `soLuong` trong hàm `main()` hoàn toàn không bị ảnh hưởng và vẫn là 0.
  - Hậu quả kép: Lần gọi tiếp theo cho `ve2` vẫn truyền `soLuong = 0`, ghi đè `ve2` lên vị trí `dsVe[0]`, đồng thời giỏ hàng ngoài `main()` kiểm tra thấy `soLuong == 0` nên thông báo rỗng.
- **Giải pháp khắc phục (trong phạm vi kiến thức Session 10 - Chưa học con trỏ)**:
  - Thay đổi kiểu trả về của hàm từ `void` thành `int`: `int datVeConcert(struct VeBan dsVe[], int soLuong, struct VeBan veMoi)`.
  - Khi thêm vé thành công, hàm trả về số lượng vé mới: `return soLuong + 1;`.
  - Nếu việc đặt vé thất bại (do đầy mảng hoặc vượt quá hạn ngạch), hàm trả về số lượng hiện tại không đổi: `return soLuong;`.
  - Trong hàm `main()`, gán nhận lại giá trị trả về để cập nhật biến đếm:
    ```c
    soLuong = datVeConcert(dsVe, soLuong, ve1);
    soLuong = datVeConcert(dsVe, soLuong, ve2);
    ```

---

#### 1.2. Sự cố 2: Thiếu kiểm tra hạn ngạch đặt vé tối đa trên mỗi đơn hàng (`MAX_VE_PER_ORDER`)
- **Hiện tượng**: Khách hàng có thể đặt liên tiếp 5 đến 6 vé trong một đơn hàng mà hệ thống không cảnh báo vi phạm hạn ngạch tối đa 4 vé/đơn.
- **Vị trí dòng lệnh sai**:
  - Dù hằng số `#define MAX_VE_PER_ORDER 4` đã được định nghĩa ở đầu chương trình, nhưng bên trong thân hàm `datVeConcert` hoàn toàn không có câu lệnh kiểm tra điều kiện này.
  - Hàm chỉ kiểm tra sức chứa tối đa của mảng:
    ```c
    if (soLuong >= MAX_VE) {
        printf("[LỖI]: Mảng lưu trữ vé của hệ thống đã đầy!\n");
        return;
    }
    ```
- **Nguyên nhân kỹ thuật**:
  - Bỏ sót quy tắc nghiệp vụ (Business Rule) về giới hạn số lượng giao dịch: `MAX_VE_PER_ORDER` (4 vé/đơn) khác với `MAX_VE` (dung lượng lưu trữ của hệ thống là 100 vé).
  - Không có điều kiện chặn khi `soLuong >= MAX_VE_PER_ORDER`, dẫn đến luồng thực thi vẫn tiếp tục ghi vé mới vào mảng.
- **Giải pháp khắc phục**:
  - Bổ sung kiểm tra điều kiện biên trước khi tiến hành lưu vé:
    ```c
    if (soLuong >= MAX_VE_PER_ORDER) {
        printf("[LỖI]: Đơn hàng đã đạt giới hạn tối đa (%d vé/đơn). Không thể đặt thêm vé %d!\n", MAX_VE_PER_ORDER, veMoi.maVe);
        return soLuong;
    }
    ```

---

### 2. Bảng Test Cases Đối Chứng

| STT | Trường hợp kiểm thử | Dữ liệu đầu vào | Kết quả sai thực tế (Mã nguồn cũ) | Kết quả đúng mong đợi (Sau khi sửa) |
| :---: | :--- | :--- | :--- | :--- |
| **TC01** | Đặt 2 vé hợp lệ vào giỏ hàng (Kiểm tra cập nhật `soLuong` & áp dụng Early Bird) | - `ve1`: Mã 101, VIP, giá 2.000.000, `isEarlyBird = 1`<br>- `ve2`: Mã 102, Zone A, giá 1.200.000, `isEarlyBird = 0` | - Thông báo đặt vé thành công.<br>- Giỏ hàng ngoài `main()` hiển thị: `[THÔNG BÁO]: Hiện chưa có vé nào được đặt thành công trong hệ thống!` (`soLuong` vẫn là 0). | - Thông báo thêm vé thành công.<br>- Giỏ hàng hiển thị đúng 2 vé:<br>  + Vé 1: Mã 101, VIP, Giá: 1.700.000 VND (đã giảm 15%).<br>  + Vé 2: Mã 102, Zone A, Giá: 1.200.000 VND.<br>- Biến `soLuong` ngoài `main()` cập nhật chính xác bằng 2. |
| **TC02** | Đặt vé vượt quá hạn ngạch cho phép (Cố gắng đặt vé thứ 5 khi hạn ngạch là 4 vé/đơn) | - Giỏ hàng đã có 4 vé (mã 101 đến 104).<br>- Gọi đặt thêm vé thứ 5 (`ve5`: Mã 105, GA, 800.000, `isEarlyBird = 0`). | - Hệ thống vẫn cho phép đặt vé thứ 5 thành công.<br>- Không có thông báo lỗi vi phạm hạn ngạch tối đa 4 vé/đơn. | - Hệ thống từ chối đặt vé thứ 5.<br>- Xuất cảnh báo: `[LỖI]: Đơn hàng đã đạt giới hạn tối đa (4 vé/đơn). Không thể đặt thêm vé 105!`<br>- `soLuong` giữ nguyên là 4 vé. |
