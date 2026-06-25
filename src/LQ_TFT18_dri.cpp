/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@��   д������Ƽ�
@��   �䣺chiusir@163.com
@����IDE��Linux ������VSCode_1.93 �����ϰ汾��Cmake_3.16 �����ϰ汾
@ʹ��ƽ̨����о2K0300�þ��ɺͱ����������ܿƼ���о�þ�����չ��
@�����Ϣ�ο����е�ַ
    ��      վ��http://www.lqist.cn
    �� �� �� �̣�http://longqiu.taobao.com
    ����������Ƶ��https://space.bilibili.com/95313236
@�����汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ

@�޸����ڣ�2025-04-28
@�޸����ݣ�
@ע�����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "LQ_TFT18_dri.hpp"

static struct tft_spi_display tft18_spi;

/*!
 * @brief   �������ݸ�ֵ
 * @param   s     : ��Ҫ��ֵ�Ŀռ�ͷָ��
 * @param   c     : ��Ҫ��ֵ��ֵ(uint16_t����)
 * @param   count : ��Ҫ��ֵ��������
 * @return  ��
 * @note    ����޸Ĺܽ� ��Ҫ�޸ĳ�ʼ���Ĺܽ�
 * @see     memset16(buf, u16RED, 10);
 * @date    2025/4/28
 */
void memset16(void *s, uint16_t c, size_t count)
{
    uint16_t *p = (uint16_t*)s;
    for (size_t i = 0; i < count; i++)
    {
        p[i] = __builtin_bswap16(c);
    }
}

/*!
 * @brief    TFT18��ʼ��
 * @param    type �� 0:����  1������
 * @return   ��
 * @note     ����޸Ĺܽ� ��Ҫ�޸ĳ�ʼ���Ĺܽ�
 * @see      TFTSPI_Init(1);
 * @date     2025/4/28
 */
void TFTSPI_dri_init(uint8_t type)
{
    // ��ȡ��Ļ�ļ�������
    tft18_spi.tft_fd = open("/dev/LQ_TFT_1.8", O_RDWR);
    if (tft18_spi.tft_fd < 0)
    {
        printf("Open file error\n");
    }
    // ӳ����Ļ������
    tft18_spi.tft_fb = (uint16_t*)mmap(NULL, FB_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, tft18_spi.tft_fd, 0);
    if (tft18_spi.tft_fb == MAP_FAILED)
    {
        perror("mmap failed");
        close(tft18_spi.tft_fd);
        return;
    }
    // ѡ���������ʾ
    switch (type)
    {
        case 0:ioctl(tft18_spi.tft_fd, IOCTL_TFT_L_INIT);break;
        case 1:ioctl(tft18_spi.tft_fd, IOCTL_TFT_V_INIT);break;
        default:
            printf("TFTSPI_dri_init error\n");
            break;
    }
    struct Pixel {
        uint8_t tft18_w;
        uint8_t tft18_h; 
    };
    struct Pixel pix;
    ssize_t read_bytes = read(tft18_spi.tft_fd, &pix, sizeof(pix));
    if (read_bytes != sizeof(pix))
    {
        perror("read failed");
        close(tft18_spi.tft_fd);
        return;
    }
    tft18_spi.tft18_h = pix.tft18_h;
    tft18_spi.tft18_w = pix.tft18_w;
    // printf("type = %d, H = %d, W = %d\n", type, tft18_spi.tft18_h, tft18_spi.tft18_w);
}

/*!
 * @brief    �޸�ָ�����������
 * @param    x ��������
 * @param    y ��������
 * @param    color ����ɫ
 * @return   ��
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 * @see      TFTSPI_dri_data_mod(10, 20, u16YELLOW);
 * @date     2025/4/28
 */
void TFTSPI_dri_data_mod(uint8_t x, uint8_t y, uint16_t color)
{
    tft18_spi.tft_fb[y * tft18_spi.tft18_w + x] = __builtin_bswap16(color);
}

/*!
 * @brief    ȫ����ʾ��ɫ����
 * @param    color ��������ɫ
 * @return   ��
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 * @see      TFTSPI_dir_cls(u16YELLOW);
 * @date     2025/4/28
 */
void TFTSPI_dir_cls(uint16_t color_dat)
{
    memset16(tft18_spi.tft_fb, color_dat, SSIZE);
}

/*!
 * @brief    ���ָ������
 * @param    xs ����ʼx
 * @param    ys ����ʼy
 * @param    xe ������x
 * @param    ys ������y
 * @param    color ��������ɫ
 * @return   ��
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 * @see      TFTSPI_dri_fill_area(10, 20, 30, 40, u16YELLOW);
 * @date     2025/4/28
 */
void TFTSPI_dri_fill_area(uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye, uint16_t color_dat)
{
    uint8_t i, j;
    for (j = 0; j < (ye - ys + 1); j++)
        for (i = 0; i < (xe - xs + 1); i++)
            TFTSPI_dri_data_mod(xs + i, ys + j, color_dat);
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    �����α߿�
 * @param    xs ����ʼx
 * @param    ys ����ʼy
 * @param    xe ������x
 * @param    ys ������y
 * @param    color_dat ����ɫ
 * @return   ��
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 * @see      TFTSPI_dri_draw_rectangle(10, 20, 30, 40, u16YELLOW);
 * @date     2025/4/28
 */
void TFTSPI_dri_draw_rectangle(uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye, uint16_t color_dat)
{
    TFTSPI_dri_draw_line(xs, ys, xs, ye, color_dat);    // ���������
    TFTSPI_dri_draw_line(xe, ys, xe, ye, color_dat);    // �������ұ�
    TFTSPI_dri_draw_line(xs, ys, xe, ys, color_dat);    // �������ϱ�
    TFTSPI_dri_draw_line(xs, ye, xe, ye, color_dat);    // �������±�
}

/*!
 * @brief   �ж������Ƿ񳬳��涨��Χ
 * @param   coor : ����ֵ
 * @param   min  : ������Сֵ
 * @param   max  : �������ֵ
 * @return  �������� 0��δ�������� 1
 */
uint8_t JudgeBeyondRange(uint8_t coor, uint8_t min, uint8_t max)
{
    if ((coor < min) || (coor > max - 1))
        return 0;
    return 1;
}

/*!
 * @brief    ��Բ
 * @param    x ��Բ��x   (0-127)
 * @param    y ��Բ��y   (0-159)
 * @param    r ���뾶    (0-128)
 * @param    color_dat ����ɫ
 * @return   ��
 * @note     Բ�����겻Ҫ������Ļ��Χ
 * @see      TFTSPI_dri_draw_circle(50, 50, 30, u16YELLOW);
 * @date     2025/4/28
 */
void TFTSPI_dri_draw_circle(uint8_t x, uint8_t y, uint8_t r, uint16_t color_dat)
{
    uint8_t dx, dy = r;
    if ((x < 0) || (x > tft18_spi.tft18_w - 1) || (y < 0) || (y > tft18_spi.tft18_h - 1))
        return;
    for (dx = 0; dx <= r; dx++)
    {
        while ((r * r + 1 - dx * dx) < (dy * dy))
            dy--;
        if (JudgeBeyondRange(x + dx, 0, tft18_spi.tft18_w) && JudgeBeyondRange(y - dy, 0, tft18_spi.tft18_h))
            TFTSPI_dri_data_mod(x + dx, y - dy, color_dat);
        if (JudgeBeyondRange(x - dx, 0, tft18_spi.tft18_w) && JudgeBeyondRange(y - dy, 0, tft18_spi.tft18_h))
            TFTSPI_dri_data_mod(x - dx, y - dy, color_dat);
        if (JudgeBeyondRange(x - dx, 0, tft18_spi.tft18_w) && JudgeBeyondRange(y + dy, 0, tft18_spi.tft18_h))
            TFTSPI_dri_data_mod(x - dx, y + dy, color_dat);
        if (JudgeBeyondRange(x + dx, 0, tft18_spi.tft18_w) && JudgeBeyondRange(y + dy, 0, tft18_spi.tft18_h))
            TFTSPI_dri_data_mod(x + dx, y + dy, color_dat);
        
        if (JudgeBeyondRange(x + dy, 0, tft18_spi.tft18_w) && JudgeBeyondRange(y - dx, 0, tft18_spi.tft18_h))
            TFTSPI_dri_data_mod(x + dy, y - dx, color_dat);
        if (JudgeBeyondRange(x - dy, 0, tft18_spi.tft18_w) && JudgeBeyondRange(y - dx, 0, tft18_spi.tft18_h))
            TFTSPI_dri_data_mod(x - dy, y - dx, color_dat);
        if (JudgeBeyondRange(x - dy, 0, tft18_spi.tft18_w) && JudgeBeyondRange(y + dx, 0, tft18_spi.tft18_h))
            TFTSPI_dri_data_mod(x - dy, y + dx, color_dat);
        if (JudgeBeyondRange(x + dy, 0, tft18_spi.tft18_w) && JudgeBeyondRange(y + dx, 0, tft18_spi.tft18_h))
            TFTSPI_dri_data_mod(x + dy, y + dx, color_dat);
    }
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    ����
 * @param    xs ����ʼx
 * @param    ys ����ʼy
 * @param    xe ������x
 * @param    ys ������y
 * @param    color_dat ����ɫ
 * @return   ��
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 * @see      TFTSPI_dri_draw_line(10, 20, 30, 40, u16YELLOW);
 * @date     2025/4/28
 */
void TFTSPI_dri_draw_line(uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye, uint16_t color)
{
    int i, ds;
    int dx, dy, inc_x, inc_y;
    int xerr = 0, yerr = 0; // ��ʼ������
    // �����������곬����Χ���򲻻ử��
    if ((xs < 0) || (xs > tft18_spi.tft18_w - 1) || (ys < 0) || (ys > tft18_spi.tft18_h - 1))
        return;
    if ((xe < 0) || (xe > tft18_spi.tft18_w - 1) || (ye < 0) || (ye > tft18_spi.tft18_h - 1))
        return;
    if (xs == xe)   // ����ǻ�ֱ����ֻ��Ҫ����ֱ�������
    {
        for (i = 0; i < (ye - ys + 1); i++)
            TFTSPI_dri_data_mod(xs, ys + i, color);
    }
    else if (ys == ye)  // �����ˮƽ����ֻ��Ҫ��ˮƽ�������
    {
        for (i = 0; i < (xe - xs + 1); i++)
            TFTSPI_dri_data_mod(xs + i, ys, color);
    }
    else    // �����б�ߣ������¼��㣬ʹ�û��㺯������ֱ��
    {
        dx = xe - xs;   // ������������
        dy = ye - ys;
        if (dx > 0)
            inc_x = 1;  // ���õ�������
        else
        {
            inc_x = -1;
            dx = -dx;
        }
        if (dy > 0)
            inc_y = 1;  // ���õ�������
        else
        {
            inc_y = -1;
            dy = -dy;
        }
        if (dx > dy)
            ds = dx;    // ѡȡ������������ֵ
        else
            ds = dy;
        for (i = 0; i <= ds + 1; i++)   // �������
        {
            TFTSPI_dri_data_mod(xs, ys, color);
            xerr += dx;
            yerr += dy;
            if (xerr > ds)
            {
                xerr -= ds;
                xs += inc_x;
            }
            if (yerr > ds)
            {
                yerr -= ds;
                ys += inc_y;
            }
        }
    }
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    ����
 * @param    x ��x
 * @param    y ��y
 * @param    color_dat ����ɫ
 * @return   ��
 * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
 * @see      TFTSPI_dri_draw_dot(10, 20, u16YELLOW);
 * @date     2025/4/28
 */
void TFTSPI_dri_draw_dot(uint8_t x, uint8_t y, uint16_t color)
{
    TFTSPI_dri_data_mod(x, y, color);
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    Һ���ַ����(6*8����)
 * @param    x: 0 - 20	(��)
 * @param    y: 0 - 19	(��)
 * @param    word_color: ������ɫ
 * @param    back_color: ������ɫ
 * @return   ��
 * @note     �ڲ�����
 * @date     2025/4/28
 */
void TFTSPI_dir_P6X8(uint8_t x, uint8_t y, uint8_t c_dat, uint16_t word_color, uint16_t back_color)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 6; i++)
        {
            if ((Font_code8[c_dat - 32][i]) & (0x01 << j))
                TFTSPI_dri_data_mod(x * 6 + i, y * 8 + j, word_color);
            else
                TFTSPI_dri_data_mod(x * 6 + i, y * 8 + j, back_color);
        }
    }
}

/*!
 * @brief    Һ���ַ����(8*8����)
 * @param    x:0 - 15	(��)
 * @param    y:0 - 19	(��)
 * @param    word_color: ������ɫ
 * @param    back_color: ������ɫ
 * @return   ��
 * @note     �ڲ�����
 * @date     2025/4/28
 */
void TFTSPI_dir_P8X8(uint8_t x, uint8_t y, uint8_t c_dat, uint16_t word_color, uint16_t back_color)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        TFTSPI_dri_data_mod(x * 8, y * 8 + j, back_color);
        for (i = 0; i < 6; i++)
        {
            if ((Font_code8[c_dat - 32][i]) & (0x01 << j))
                TFTSPI_dri_data_mod(x * 8 + i + 1, y * 8 + j, word_color);
            else
                TFTSPI_dri_data_mod(x * 8 + i + 1, y * 8 + j, back_color);
        }
        TFTSPI_dri_data_mod(x * 8 + 7, y * 8 + j, back_color);
    }
}

/*!
 * @brief    Һ���ַ����(8*16����)
 * @param    x: 0 -15   (��)
 * @param    y: 0 -9  	 (��)
 * @param    word_color: ������ɫ
 * @param    back_color: ������ɫ
 * @return   ��
 * @note     �ڲ�����
 * @date     2025/4/28
 */
void TFTSPI_dir_P8X16(uint8_t x, uint8_t y, uint8_t c_dat, uint16_t word_color, uint16_t back_color)
{
    uint8_t i, j;
    for (j = 0; j < 16; j++)
    {
        for (i = 0; i < 8; i++)
        {
            if ((Font_code16[c_dat - 32][j]) & (0x01 << i))
                TFTSPI_dri_data_mod(x * 8 + i, y * 16 + j, word_color);
            else
                TFTSPI_dri_data_mod(x * 8 + i, y * 16 + j, back_color);
        }
    }
}

/*!
 * @brief    Һ���ַ������(6*8����)
 * @param    x: 0 - 20 (��)
 * @param    y: 0 - 19 (��)
 * @param    word_color: ������ɫ
 * @param    back_color: ������ɫ
 * @return   ��
 * @note     ��
 * @see      TFTSPI_dir_P6X8Str(1, 1, "123456", u16YELLOW, u16RED);
 * @date     2025/4/28
 */
void TFTSPI_dir_P6X8Str(uint8_t x, uint8_t y, const char *s_dat, uint16_t word_color, uint16_t back_color)
{
    while (*s_dat)
        TFTSPI_dir_P6X8(x++, y, *s_dat++, word_color, back_color);
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    Һ���ַ������(8*8����)
 * @param    x:0 - 15 (��)
 * @param    y:0 - 19 (��)
 * @param    word_color: ������ɫ
 * @param    back_color: ������ɫ
 * @return   ��
 * @note     ��
 * @see      TFTSPI_dir_P8X8Str(1, 1, "123456", u16YELLOW, u16RED);
 * @date     2025/4/28
 */
void TFTSPI_dir_P8X8Str(uint8_t x, uint8_t y, const char *s_dat, uint16_t word_color, uint16_t back_color)
{
    while (*s_dat)
        TFTSPI_dir_P8X8(x++, y, *s_dat++, word_color, back_color);
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    Һ���ַ������(8*16����)
 * @param    x: x: 0 -15   (��)
 * @param    y: y: 0 -9  	 (��)
 * @param    word_color: ������ɫ
 * @param    back_color: ������ɫ
 * @return   ��
 * @note     ��
 * @see      TFTSPI_dir_P8X16Str(1, 1, "123456", u16YELLOW, u16RED);
 * @date     2025/4/28
 */
void TFTSPI_dir_P8X16Str(uint8_t x, uint8_t y, const char *s_dat, uint16_t word_color, uint16_t back_color)
{
    while (*s_dat)
        TFTSPI_dir_P8X16(x++, y, *s_dat++, word_color, back_color);
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    Һ�������ַ������(16*16����)
 * @param    x: 0 - 7	(��)
 * @param    y: 0 - 9	(��)
 * @param    word_color: ������ɫ
 * @param    back_color: ������ɫ
 * @return   ��
 * @note     ����ֻ�����ֿ���� �ֿ�û�е���Ҫ��������
 * @see      TFTSPI_dir_P16x16Str(1, 1, "123456", u16YELLOW, u16RED);
 * @date     2025/4/28
 */
void TFTSPI_dir_P16x16Str(uint8_t x, uint8_t y, const char *s_dat, uint16_t word_color, uint16_t back_color)
{
    uint8_t wm = 0, ii = 0, i, j;
    int adder = 1;
    while (s_dat[ii] != '\0')
    {
        wm = 0;
        adder = 1;
        while (hanzi_Idx[wm] > 127)
        {
            if (hanzi_Idx[wm] == (uint8_t)s_dat[ii])
            {
                if (hanzi_Idx[wm + 1] == s_dat[ii + 1])
                {
                    adder = wm * 16;
                    break;
                }
            }
            wm += 2;
        }

        if (adder != 1) // ��ʾ����
        {
            for (j = 0; j < 32; j++)
            {
                for (i = 0; i < 8; i++)
                {
                    if ((hanzi16x16[adder]) & (0x80 >> i))
                    {
                        TFTSPI_dri_data_mod(x * 16 + i + (j % 2) * 8, y * 16 + (j / 2), word_color);
                    }
                    else
                    {
                        TFTSPI_dri_data_mod(x * 16 + i + (j % 2) * 8, y * 16 + (j / 2), back_color);
                    }
                }
                adder += 1;
            }
        }
        else // ��ʾ�հ��ַ�
        {
        }
        // y+=1;//���ҷ���
        x += 1; // ���·���
        ii += 2;
    }
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    TFT18�� unsigned char �Ҷ�������ʾ
 * @param    high_start �� ��ʾͼ��ʼλ��
 * @param    wide_start �� ��ʾͼ��ʼλ��
 * @param    high �� ��ʾͼ��߶�
 * @param    wide �� ��ʾͼ�����
 * @param    Pixle�� ��ʾͼ�����ݵ�ַ
 * @return   ��
 * @note     ע�� ��Ļ����Ϊ ��0��0��
 * @see
 * @date     2025/4/28
 */
void TFTSPI_dir_road(uint8_t wide_start, uint8_t high_start, uint8_t high, uint8_t wide, uint8_t *Pixle)
{
    uint64_t i, j;
    uint16_t color;
    for (j = 0; j < high; j++)
    {
        for (i = 0; i < wide; i++)
        {
            /* ���Ҷ�ת��Ϊ RGB565 */
            color = (Pixle[j * wide + i] >> 3) << 11;
            color |= (Pixle[j * wide + i] >> 2) << 5;
            color |= Pixle[j * wide + i] >> 3;
            TFTSPI_dri_data_mod(wide_start + i, high_start + j, color);
        }
    }
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}

/*!
 * @brief    TFT18�� unsigned char ��ֵ��������ʾ
 * @param    high_start �� ��ʾͼ��ʼλ��
 * @param    wide_start �� ��ʾͼ��ʼλ��
 * @param    high �� ��ʾͼ��߶�
 * @param    wide �� ��ʾͼ�����
 * @param    Pixle�� ��ʾͼ�����ݵ�ַ
 * @return   ��
 * @note     ע�� ��Ļ����Ϊ ��0��0��
 * @see
 * @date     2025/4/28
 */
void TFTSPI_dir_binRoad(uint8_t wide_start, uint8_t high_start, uint8_t high, uint8_t wide, uint8_t *Pixle)
{
    uint8_t i, j;
    /* ��ʾͼ�� */
    for (j = 0; j < high; j++)
    {
        for (i = 0; i < wide; i++)
        {
            if (Pixle[j * wide + i])
                TFTSPI_dri_data_mod(wide_start + i, high_start + j, u16WHITE); /* ��ʾ */
            else
                TFTSPI_dri_data_mod(wide_start + i, high_start + j, u16BLACK);
        }
    }
    TFTSPI_dir_flush();
}

/*!
 * @brief    TFT18�� ˢ����Ļ
 * @param    ��
 * @return   ��
 * @note     ��
 * @date     2025/4/28
 */
void TFTSPI_dir_flush()
{
    ioctl(tft18_spi.tft_fd, IOCTL_TFT_FLUSH);
}
