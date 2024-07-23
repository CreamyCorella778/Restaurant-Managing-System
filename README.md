# Restaurant-Managing-System
Back-end: C++
UX/UI: ???
Database: Microsoft SQL Server

Description (In Vietnamese):

- Nhân viên:
  + Đăng nhập và hiển thị, thay đổi thông tin tài khoản nhân viên quản lý web
  + Tạo chi nhánh mới, tạo chỗ mới
  + Thay đổi thông tin chỗ, hủy chỗ
  + Theo dõi trạng thái của bàn, sắp xếp và gán bàn theo yêu cầu người dùng, quản lý số lượng bàn và khả năng chứa của từng bàn
  + Thêm mới, thay đổi, xóa thực đơn theo bàn
  + Tính toán giá tiền đặt bàn theo thực đơn và các chi phí phát sinh
  + Tạo, hiển thị và in hóa đơn cho người dùng
  + Theo dõi doanh thu và thống kê theo điều kiện
  + Quản lý các đặc quyền của VIP
  + Quản lý các chương trình khuyến mãi theo thời gian
- Người dùng:
  + Đăng nhập và hiển thị, thay đổi thông tin tài khoản người dùng
  + Đặt bàn (Tìm kiếm và hiển thị thông tin chi nhánh): Chọn ngày/giờ, chọn thương hiệu đồ ăn, chọn tỉnh/thành phố/phường
  + Hủy hoặc thay đổi thông tin đặt bàn (theo thời hạn trước xxx ngày kể từ ngày đi ăn)
  + Đăng ký tài khoản VIP với điều kiện tích lũy tổng tiền đã sử dụng phải trên xxx tiền => ...
  + Xem thực đơn
  + Xem lịch sử đặt bàn
  + Đánh giá bàn và trải nghiệm ăn uống tại chi nhánh
  + Xem hóa đơn điện tử với tùy chọn in

- Giao diện:
  + Hiển thị quy trình đặt bàn
  + Bàn: Hiển thị ngày giờ, địa chỉ rõ ràng, số điện thoại liên hệ
  + ...

Mô tả CSDL
- Một chuỗi nhà hàng của một thương hiệu rất lớn mở nhiều chi nhánh,
mỗi chi nhánh gồm: mã cn, tên cn, địa chỉ cn, số điện thoại liên lạc, giờ mở/đóng cửa, một trong số các thương hiệu con của thương hiệu lớn.
Một thương hiệu con có thể có nhiều chi nhánh.
- Mỗi chi nhánh là một nhà hàng gồm nhiều bàn ăn để ăn,
mỗi bàn ăn gồm: tọa độ bàn ăn (duy nhất trong một chi nhánh), số lượng ghế.
- Khách hàng khi đặt bàn ăn có thể chọn thực đơn cho bàn ăn, 
mỗi thực đơn gồm mã thực đơn, danh sách các tên món ăn, giá tiền của cả thực đơn. Một thực đơn có thể thuộc về nhiều bàn ăn khác nhau. 
Khách hàng sẽ chọn thực đơn trong số các thực đơn có sẵn chứ không tùy hứng chọn món ăn để ghép vào 1 thực đơn theo ý muốn.
Nhân viên có thể tạo một thực đơn mới và thay đổi món ăn trong thực đơn, nhưng phải đảm bảo đủ 3 loại: Khai vị, Chính, Tráng miệng. 
(Nói cách khác, món ăn bao gồm các thông tin: mã món (duy nhất trong cả hệ thống), tên món, loại món)
Danh sách thực đơn là đặc trưng của một thương hiệu con.
- Có hai loại tài khoản: Nhân viên và Khách hàng
Nhân viên có thể quản lý một và chỉ một chi nhánh nhất định (Một chi nhánh có thể có nhiều nhân viên). Các thông tin khác của nhân viên gồm mã nv (duy nhất trong một chi nhánh), tên nv, số điện thoại, năm bắt đầu làm việc.
Nhân viên có thể thống kê doanh thu của một chi nhánh nào đó.
Các nhân viên cùng chi nhánh chịu sự quản lý của một nhân viên quản lý thương hiệu con. Người này còn phải thống kê doanh thu của thương hiệu con này. 
- Khách hàng chỉ có thể đặt 1 bàn trong 1 lần (thời điểm), mỗi lần đặt sẽ có hóa đơn. 
Các thông tin khác của khách hàng gồm mã kh (duy nhất trên cả hệ thống), tên kh, số điện thoại, loại kh (thường hay VIP).
Việc đặt bàn ăn còn phải chỉ rõ số người ăn, và nó phải không lớn hơn số ghế của bàn ăn.
Một bàn chỉ tương ứng 1 khách hàng, đã có khách rồi thì không nhận thêm người.
Các thông tin trên hóa đơn: mã hd (duy nhất trên cả hệ thống), ngày giờ đặt, ngày giờ ăn, số người ăn, giá tiền tổng (khi hiển thị phải có in ra các giá tiền thành phần), thông tin của kh, cn, bàn ăn, thực đơn.
- Khách hàng ăn xong có thể để lại rì viu đánh giá. Rì viu gồm số sao và văn bản rì viu chi tiết.
- Khách hàng có hai loại: Thường và VIP. VIP sẽ có đặc quyền giảm giá theo từng lần đặt. Để đạt được VIP, phải có tích lũy cả số lần đặt và tổng tiền đặt (cho một thương hiệu con).



Cơ sở dữ liệu quan hệ:
- Bàn ăn(Mã CN, Tọa độ BA, Số lượng ghế)
- Chi nhánh(Mã CN, Tên CN, Địa chỉ CN, SDT, Giờ mở cửa, Giờ đóng cửa, Thương hiệu con)
- Khách hàng(Mã KH, Tên KH, SDT, Loại KH)
- Nhân viên(Mã NV, Tên NV, SDT, Năm bắt đầu, Mã CN, Mã NVQL)
- Thực đơn(Mã TD, Giá tiền)
- Món ăn(Mã MA, Tên MA, Loại MA)
- Review(Mã HD, Số sao, Chi tiết)
- Hóa đơn(Mã HD, Ngày giờ đặt, Ngày giờ ăn, Số người ăn, Mã CN, Tọa độ BA, Mã KH)
- CT_Bàn ăn(Mã CN, Tọa độ BA, Mã TD)
- CT_Thực đơn(Mã TD, Mã MA)


Sơ đồ lớp OOP
https://lucid.app/lucidchart/94a2bd85-37db-458f-8cbf-4ab3fc3b7d43/edit?viewport_loc=-816%2C577%2C2686%2C1389%2CHWEp-vi-RSFO&invitationId=inv_97b55cfb-a262-478c-adf5-3f47e1455fd4
