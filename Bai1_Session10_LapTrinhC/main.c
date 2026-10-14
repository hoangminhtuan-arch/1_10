#include <stdio.h>
#include <string.h>

#define MAX_VE 100
#define MAX_VE_PER_ORDER 4

struct VeBan {
    int maVe;
    char tenKhuVuc[20];
    float giaGoc;
    int isEarlyBird;
};

int datVeConcert(struct VeBan dsVe[], int soLuong, struct VeBan veMoi);
void hienThiDanhSachVe(const struct VeBan dsVe[], int soLuong);

int main() {
    struct VeBan dsVe[MAX_VE];
    int soLuong = 0;

    printf("=== HE THONG DAT VE CONCERT TICKETBOX ===\n\n");

    struct VeBan ve1 = {101, "VIP", 2000000.0f, 1};
    struct VeBan ve2 = {102, "Zone A", 1200000.0f, 0};

    soLuong = datVeConcert(dsVe, soLuong, ve1);
    soLuong = datVeConcert(dsVe, soLuong, ve2);

    printf("\n>>> KET QUA KIEM TRA GIO HANG TRONG MAIN <<<\n");
    hienThiDanhSachVe(dsVe, soLuong);

    return 0;
}

int datVeConcert(struct VeBan dsVe[], int soLuong, struct VeBan veMoi) {
    if (soLuong >= MAX_VE) {
        printf("[LOI]: Mang luu tru ve cua he thong da day!\n");
        return soLuong;
    }

    if (soLuong >= MAX_VE_PER_ORDER) {
        printf("[LOI]: Don hang da dat toi da %d ve!\n", MAX_VE_PER_ORDER);
        return soLuong;
    }

    if (veMoi.isEarlyBird == 1) {
        veMoi.giaGoc = veMoi.giaGoc * 0.85f;
    }

    dsVe[soLuong] = veMoi;
    soLuong++;
    printf("[THANH CONG]: Da them ve %d (%s) vao don hang.\n", veMoi.maVe, veMoi.tenKhuVuc);

    return soLuong;
}

void hienThiDanhSachVe(const struct VeBan dsVe[], int soLuong) {
    if (soLuong == 0) {
        printf("[THONG BAO]: Hien chua co ve nao duoc dat thanh cong trong he thong!\n");
        return;
    }

    printf("--- DANH SACH VE DA DAT (%d ve) ---\n", soLuong);
    for (int i = 0; i < soLuong; i++) {
        printf("STT: %d | Ma ve: %d | Khu vuc: %s | Gia thanh toan: %.0f VND\n",
               i + 1, dsVe[i].maVe, dsVe[i].tenKhuVuc, dsVe[i].giaGoc);
    }
}
