-- Tạo mới cơ sở dữ liệu
CREATE DATABASE QLNH;
GO

-- Sử dụng cơ sở dữ liệu QLNH
USE QLNH;
GO

-- Tạo bảng Bàn ăn
CREATE TABLE BanAn (
    MaCN CHAR(10),
    ToaDoBA CHAR(10),
    SoLuongGhe INT,
	primary key (macn, toadoba)
);
GO

-- Tạo bảng Chi nhánh
CREATE TABLE ChiNhanh (
    MaCN CHAR(10) PRIMARY KEY,
    TenCN NVARCHAR(50),
    DiaChiCN NVARCHAR(500),
    SDT NVARCHAR(10),
    GioMoCua TIME,
    GioDongCua TIME,
    ThuongHieuCon NVARCHAR(100)
);
GO

alter table banan add foreign key (macn) references chinhanh(macn)

-- Tạo bảng Khách hàng
CREATE TABLE KhachHang (
    MaKH CHAR(10) PRIMARY KEY,
    TenKH NVARCHAR(50),
    SDT NVARCHAR(10),
	MatKhau nvarchar(50),
    LoaiKH NCHAR(5),
	check (loaikh in (N'Thường', N'VIP'))
);
GO

-- Tạo bảng Nhân viên
CREATE TABLE NhanVien (
    MaNV CHAR(10) PRIMARY KEY,
    TenNV NVARCHAR(50),
    SDT NVARCHAR(10),
	MatKhau nvarchar(50),
    NamBatDau INT,
    MaCN CHAR(10),
    MaNVQL CHAR(10),
    FOREIGN KEY (MaCN) REFERENCES ChiNhanh(MaCN),
    FOREIGN KEY (MaNVQL) REFERENCES NhanVien(MaNV)
);
GO

-- Tạo bảng Thực đơn
CREATE TABLE ThucDon (
    MaTD CHAR(10) PRIMARY KEY,
    GiaTien int
);
GO

-- Tạo bảng Món ăn
CREATE TABLE MonAn (
    MaMA CHAR(10) PRIMARY KEY,
    TenMA NVARCHAR(50),
    LoaiMA NCHAR(11)
	check (loaima in (N'Khai vị', N'Chính', 'Tráng miệng'))
);
GO

-- Tạo bảng Review
CREATE TABLE Review (
    MaHD CHAR(10) PRIMARY KEY,
    SoSao Decimal,
    ChiTiet NVARCHAR(500)
	check (sosao <= 5)
);
GO

-- Tạo bảng Hóa đơn
CREATE TABLE HoaDon (
    MaHD CHAR(10) PRIMARY KEY,
    NgayGioDat DATETIME,
    NgayGioAn DATETIME,
    SoNguoiAn INT,
    MaCN CHAR(10),
    ToaDoBA CHAR(10),
    MaKH CHAR(10),
    FOREIGN KEY (MaKH) REFERENCES KhachHang(MaKH),
	foreign key (macn, toadoba) references banan(macn, toadoba)
);
GO

-- Tạo bảng CT_Bàn ăn
CREATE TABLE CT_BanAn (
    MaCN CHAR(10),
	ToaDoBA CHAR(10),
    MaTD CHAR(10),
    PRIMARY KEY (MaCN, ToaDoBA, MaTD),
    FOREIGN KEY (MaCN, toadoba) REFERENCES BanAn(MaCN, toadoba),
    FOREIGN KEY (MaTD) REFERENCES ThucDon(MaTD)
);
GO

-- Tạo bảng CT_Thực đơn
CREATE TABLE CT_ThucDon (
    MaTD CHAR(10),
    MaMA CHAR(10),
    PRIMARY KEY (MaTD, MaMA),
    FOREIGN KEY (MaTD) REFERENCES ThucDon(MaTD),
    FOREIGN KEY (MaMA) REFERENCES MonAn(MaMA)
);
GO

INSERT INTO ChiNhanh (MaCN, TenCN, DiaChiCN, SDT, GioMoCua, GioDongCua, ThuongHieuCon)
VALUES
('CN001', N'Chi nhánh 1', N'Số 2 đường Nguyễn Văn Trỗi, phường Phú Nhuận, quận Phú Nhuận, TP. Hồ Chí Minh', '028 3888 8888', '08:00:00', '22:00:00', N'Lẩu nấm'),
('CN002', N'Chi nhánh 2', N'Số 10 đường Cầu Giấy, phường Cầu Giấy, quận Cầu Giấy, TP. Hà Nội', '024 3999 9999', '07:30:00', '23:00:00', N'Lẩu bò'),
('CN003', N'Chi nhánh 3', N'Số 15 đường Lê Duẩn, phường Hải Châu, quận Hải Châu, TP. Đà Nẵng', '0511 3333 3333', '09:00:00', '22:30:00', N'Lẩu hải sản');

-- Insert dữ liệu vào bảng Bàn ăn
INSERT INTO BanAn (MaCN, ToaDoBA, SoLuongGhe) VALUES
('CN001', 'A1', 4),
('CN001', 'A2', 2),
('CN002', 'B1', 6),
('CN002', 'B2', 8),
('CN003', 'A1', 10),
('CN003', 'B1', 8);

-- Insert dữ liệu vào bảng Khách hàng
INSERT INTO KhachHang (MaKH, TenKH, SDT, MatKhau, LoaiKH) VALUES
('KH001', N'Nguyen Van A', '0908765432', '12345678', N'Thường'),
('KH002', N'Tran Thi B', '0912345678', 'qwert', N'VIP');

-- Insert dữ liệu vào bảng Nhân viên
INSERT INTO NhanVien (MaNV, TenNV, SDT, MatKhau, NamBatDau, MaCN, MaNVQL)
VALUES
('NV001', N'Nguyễn Văn Nam', '0987654321', '123456789', 2020, 'CN01', NULL),
('NV002', N'Trần Thị Lan', '0901234567', 'qwert', 2021, 'CN02', NULL),
('NV003', N'Lê Văn Hải', '0938475612', 'ndong3332', 2022, 'CN01', 'NV01'),
('NV004', N'Đặng Thị Mai', '0965432109', 'tongbithu', 2023, 'CN01', 'NV01'),
('NV005', N'Phạm Thị Hoa', '0912345678', 'vocungthuongtiec0000', 2024, 'CN02', 'NV02');

-- Insert dữ liệu vào bảng Thực đơn
INSERT INTO ThucDon (MaTD, GiaTien) VALUES
('TD001', 100000),
('TD002', 200000);

-- Insert dữ liệu vào bảng Món ăn
INSERT INTO MonAn (MaMA, TenMA, LoaiMA) VALUES
('MA001', N'Gỏi cuốn', N'Khai vị'),
('MA002', N'Phở bò', N'Chính'),
('MA003', N'Chè khúc bạch', N'Tráng miệng');

-- Insert dữ liệu vào bảng Review
INSERT INTO Review (MaHD, SoSao, ChiTiet) VALUES
('HD001', 4.5, N'Món ăn ngon, phục vụ tốt'),
('HD002', 3.0, N'Dịch vụ trung bình, món ăn ổn');

-- Insert dữ liệu vào bảng Hóa đơn
INSERT INTO HoaDon (MaHD, NgayGioDat, NgayGioAn, SoNguoiAn, MaCN, ToaDoBA, MaKH) VALUES
('HD001', '2024-07-01 12:00:00', '2024-07-01 18:00:00', 4, 'CN001', 'A1', 'KH001'),
('HD002', '2024-07-02 13:00:00', '2024-07-02 19:00:00', 2, 'CN002', 'B1', 'KH002');

-- Insert dữ liệu vào bảng CT_Bàn ăn
INSERT INTO CT_BanAn (MaCN, ToaDoBA, MaTD) VALUES
('CN001', 'A1', 'TD001'),
('CN002', 'B1', 'TD002');

-- Insert dữ liệu vào bảng CT_Thực đơn
INSERT INTO CT_ThucDon (MaTD, MaMA) VALUES
('TD001', 'MA001'),
('TD001', 'MA002'),
('TD002', 'MA003');
