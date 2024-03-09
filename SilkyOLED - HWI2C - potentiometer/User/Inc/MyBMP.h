//
// Created by 86159 on 2024-02-22.
//

#ifndef MY_FLYBACK_MYBMP_H
#define MY_FLYBACK_MYBMP_H

//64*64
const unsigned char BMP_YoKo[512] = { /* 0X20,0X01,0X40,0X00,0X40,0X00, */
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X06,0X00,0X00,0X00,0X00,0XF0,0X03,0X80,0X05,
        0X00,0X00,0X00,0X00,0XFC,0X0F,0X65,0X06,0X00,0X00,0X00,0X00,0XFF,0X1F,0X1B,0X03,
        0X00,0X00,0X00,0X80,0XFF,0XBF,0XE4,0X01,0X00,0X00,0X00,0XC0,0XFF,0XFF,0X33,0X01,
        0X00,0X00,0X00,0XC0,0XFF,0X7F,0X91,0X00,0X00,0X00,0X00,0XE0,0XFF,0XFF,0XD9,0X00,
        0X00,0X20,0X00,0XE0,0XFF,0XFF,0X49,0X02,0X00,0X30,0X00,0XE0,0XFF,0XFD,0XF9,0X07,
        0X00,0X30,0X00,0XF0,0XBF,0X7D,0XE4,0X0F,0X40,0X28,0X00,0XF0,0X5D,0XDF,0X37,0X0E,
        0X40,0X28,0X00,0XF0,0XCD,0XDF,0X30,0X0E,0X80,0X24,0X00,0XF0,0X17,0XDE,0X62,0X0E,
        0X80,0X34,0X00,0XF0,0X0F,0XDC,0XF7,0X07,0X00,0X24,0X00,0XF0,0X0B,0XFC,0XE1,0X03,
        0X80,0X20,0X00,0XF8,0X07,0X7C,0X9F,0X03,0X80,0X51,0X00,0XF8,0X05,0X30,0X9F,0X01,
        0X00,0X5B,0X00,0XF8,0X01,0XF0,0XEF,0X01,0X00,0X4A,0X00,0X70,0X02,0XB8,0XFE,0X00,
        0X00,0X49,0X00,0X70,0X03,0X30,0X7A,0X00,0X00,0X24,0X00,0X70,0X07,0XF4,0X02,0X00,
        0X00,0X3E,0X02,0X70,0X57,0XF2,0X1B,0X00,0X00,0X38,0X02,0X78,0XDE,0XFA,0X27,0X00,
        0X00,0XBC,0X07,0X78,0X3A,0XF0,0X22,0X00,0X00,0XE6,0X3F,0X68,0X8E,0XFC,0X02,0X00,
        0X00,0X22,0X00,0XE8,0X46,0XF5,0X41,0X00,0X00,0XC3,0X00,0X6C,0X47,0XB2,0X41,0X00,
        0X80,0X01,0X00,0X68,0XDD,0X3E,0X41,0X05,0XC0,0X06,0X01,0X20,0X40,0X5B,0X83,0X06,
        0X60,0X63,0X00,0X20,0X20,0X3A,0X01,0X06,0XE0,0X7F,0X00,0X24,0X10,0X3F,0X07,0X03,
        0XF0,0X03,0X00,0X24,0XC0,0X19,0X0F,0X06,0XF0,0X07,0X00,0X26,0X68,0X7A,0X05,0X06,
        0XF0,0X1F,0X00,0X20,0X11,0X3C,0X13,0X06,0XF0,0X7F,0X00,0X32,0X0D,0X3C,0XE1,0X04,
        0XF0,0X0F,0X00,0X32,0X05,0X7F,0XE5,0X05,0XF0,0X1F,0X00,0XB1,0X01,0X7F,0X81,0X00,
        0XF0,0X19,0X00,0X72,0X80,0X6F,0X89,0X06,0XF0,0X11,0X00,0X11,0XC0,0X1F,0X49,0X04,
        0XF0,0X01,0X80,0X09,0XF2,0X5F,0XB1,0X02,0XF0,0X01,0X00,0X3C,0XF8,0X5F,0X61,0X0C,
        0XF0,0X01,0X80,0XFD,0XFE,0X1F,0XD2,0X0F,0XF0,0X03,0XC0,0XFE,0XFF,0X1F,0XB2,0X0A,
        0XF0,0X03,0X40,0XFE,0XFF,0X9F,0XF2,0X03,0XF0,0X01,0XC0,0XFC,0XFF,0X1F,0XFA,0X07,
        0XF0,0X03,0X60,0XFC,0XFF,0X9F,0X2E,0X02,0XE0,0X03,0X20,0XFE,0XFF,0X9F,0X28,0X04,
        0XE0,0X07,0X30,0XFE,0XFF,0X8F,0X6C,0X0C,0XC0,0X07,0X00,0XFE,0XFF,0X8F,0XE0,0X0C,
        0X80,0X0F,0X10,0XFC,0XFF,0X87,0XD9,0X0F,0X80,0X1F,0X18,0XFF,0XFF,0X03,0XCA,0X07,
        0X00,0X1F,0X08,0X8F,0X1F,0X00,0XF2,0X07,0X00,0X3E,0X0C,0X93,0X00,0X02,0XE3,0X0F,
        0X00,0X7E,0X94,0XFE,0XA0,0X02,0XC1,0X0F,0X00,0XFC,0X86,0X7F,0XF0,0XBF,0XCD,0X0F,
        0X00,0XF8,0X8F,0X7F,0XE0,0XBF,0XDB,0X0F,0X00,0XF8,0XC3,0X7F,0X20,0X91,0X9A,0X1F,
        0X00,0XF0,0X87,0X3F,0XA0,0XC1,0XB8,0X1F,0X00,0XE0,0XDF,0X3F,0XA0,0XE1,0XF8,0X1F,
        0X00,0XC0,0XFF,0X1F,0XE0,0XC0,0XF2,0X1F,0X00,0XE0,0XFF,0X1F,0XE0,0XC1,0XF2,0X1F,
};


//102*64
const unsigned char BMP_Geass[832] = { /* 0X20,0X01,0X66,0X00,0X40,0X00, */
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X18,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X30,0X00,0X00,0X00,0X00,0X00,
        0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X07,0X00,0X00,
        0X00,0X00,0X00,0X00,0XC0,0X01,0X00,0X00,0X00,0XC0,0X03,0X00,0X00,0X00,0X00,0X00,
        0X00,0X80,0X03,0X00,0X00,0X00,0XE0,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0E,
        0X00,0X00,0X00,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3C,0X00,0X00,0X00,
        0X3E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X70,0X00,0X00,0X00,0X1F,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0X01,0X00,0XC0,0X0F,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0XC0,0X07,0X00,0XF0,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X80,0X1F,0X00,0XFC,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X00,
        0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF8,0X80,0X7F,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7C,0X80,0X3F,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X7C,0X80,0X1F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X3C,0XC0,0X1F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7E,
        0X80,0X1F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFE,0X80,0X3F,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0XC1,0X3F,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0XF8,0XE3,0X1F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0XF0,0XFF,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0XE0,0XFF,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0XFF,0X03,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XFF,0X01,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X7E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X3C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
//64*64
const unsigned char BMP_Boy[512] = { /* 0X20,0X01,0X40,0X00,0X40,0X00, */
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X8E,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X05,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X05,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X05,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFE,0X03,0X00,0X00,0X00,
        0X00,0X00,0X80,0X3F,0X3C,0X00,0X00,0X00,0X00,0X00,0XE0,0X80,0X83,0X01,0X00,0X00,
        0X00,0X00,0X10,0X0C,0X04,0X06,0X00,0X00,0X00,0X00,0X0C,0X02,0X08,0X18,0X00,0X00,
        0X00,0X00,0X06,0X01,0X12,0X20,0X00,0X00,0X00,0X00,0XAF,0X00,0X28,0X40,0X00,0X00,
        0X00,0X00,0XA7,0X00,0XD0,0X80,0X00,0X00,0X00,0X80,0X43,0X00,0X60,0X41,0X01,0X00,
        0X00,0XC0,0X41,0X10,0XC0,0X84,0X01,0X00,0X00,0X60,0X00,0X00,0X80,0X98,0X02,0X00,
        0X00,0X4E,0X20,0X09,0X82,0XC7,0X0F,0X00,0X00,0XA0,0XB2,0X08,0X02,0XF1,0X07,0X00,
        0X00,0X20,0X72,0X08,0X12,0X05,0X07,0X00,0X00,0X50,0X32,0X84,0X92,0X05,0X07,0X00,
        0X00,0XB8,0XA2,0X94,0X12,0X00,0X0F,0X00,0X00,0X26,0XA2,0X94,0X12,0X00,0X0E,0X00,
        0X00,0X60,0XE3,0X97,0X12,0X02,0X0F,0X00,0X00,0X40,0XFA,0X9E,0X9F,0X00,0X1B,0X00,
        0X00,0X60,0X1A,0X9B,0XF7,0X14,0X0A,0X00,0X00,0X70,0XFA,0X8B,0XBB,0X97,0X02,0X00,
        0X00,0XB0,0XF9,0XAF,0XB2,0X97,0X02,0X00,0X00,0XD0,0XFC,0X9F,0XBE,0XC6,0X03,0X00,
        0X00,0XC0,0X16,0XC0,0XC1,0XCF,0X03,0X00,0X00,0X40,0X1B,0X80,0XC1,0XBB,0X03,0X00,
        0X00,0X40,0X1B,0X00,0X81,0XB7,0X02,0X00,0X00,0X40,0X27,0X08,0X02,0XF7,0X02,0X00,
        0X00,0X40,0X1A,0X00,0X00,0X7B,0X00,0X00,0X00,0X80,0X16,0X00,0X00,0X5B,0X00,0X00,
        0X00,0X80,0X14,0X00,0X80,0X3A,0X00,0X00,0X00,0X00,0X21,0X00,0X00,0X3F,0X00,0X00,
        0X00,0X00,0X20,0X80,0X01,0X17,0X00,0X00,0X00,0X00,0X40,0X00,0X81,0X13,0X00,0X00,
        0X00,0X00,0X80,0X01,0X60,0X09,0X00,0X00,0X00,0X00,0X00,0X03,0X90,0X08,0X00,0X00,
        0X00,0X00,0X00,0X04,0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X18,0X06,0X00,0X00,0X00,
        0X00,0X00,0X00,0XA8,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X48,0X02,0X00,0X00,0X00,
        0X00,0X00,0X80,0X0F,0X1E,0X00,0X00,0X00,0X00,0X00,0X80,0X09,0X1A,0X00,0X00,0X00,
        0X00,0X00,0X40,0X06,0X16,0X00,0X00,0X00,0X00,0X00,0X3C,0X02,0X26,0X00,0X00,0X00,
        0X00,0X80,0X0B,0X06,0X62,0X02,0X00,0X00,0X00,0XF8,0X1F,0X04,0XC1,0X1D,0X00,0X00,
        0X00,0XFC,0X37,0X88,0XE0,0X7E,0X00,0X00,0X00,0XFC,0X2F,0XD0,0XB0,0XFF,0X03,0X00,
        0X00,0XFC,0X5F,0XF1,0X58,0XFF,0X07,0X00,0X00,0XFC,0XBF,0XE0,0XAC,0XFF,0X07,0X00,
        0X00,0XFE,0XFF,0X50,0XD9,0XFF,0X07,0X00,0X00,0XFE,0XFF,0X41,0XED,0XFF,0X07,0X00,
        0X00,0XFE,0XFF,0XF9,0XFD,0XFF,0X0F,0X00,0X00,0XFE,0XFF,0X9B,0XFF,0XFF,0X0F,0X00,
        0X00,0XFF,0XFF,0XFB,0XFF,0XFF,0X1F,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0X00,
        0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X3F,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X3F,0X00,
};

//94*64
const unsigned char BMP_Dog[768] = { /* 0X20,0X01,0X5E,0X00,0X40,0X00, */
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,
        0X1C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X60,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X80,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,
        0X00,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X68,0X00,0X04,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X08,0X1F,0X04,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X40,0X28,0X20,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X00,
        0X23,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X18,0X08,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X00,0X00,0X04,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X01,0X00,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,
        0X00,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XFB,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XA0,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X28,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFE,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X10,0X00,0X1C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0X00,
        0X3C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF8,0XC3,0X0F,0X00,0X38,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0XFE,0XC8,0X0F,0X00,0X78,0X00,0X00,0X00,0X00,0X00,0X00,0X80,
        0XFF,0XE0,0X0F,0X00,0X79,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0XFF,0XE0,0X0F,0X02,
        0XF4,0X00,0X00,0X00,0X00,0X00,0X00,0X30,0X7C,0XF0,0X0F,0X00,0XF0,0X00,0X00,0X00,
        0X00,0X00,0X00,0X28,0X00,0XF0,0X0F,0X00,0XF0,0X01,0X00,0X00,0X00,0X00,0X00,0X14,
        0X00,0XF8,0X07,0X00,0XE0,0X01,0X00,0X00,0X00,0X00,0X00,0X12,0X00,0XF8,0X07,0X00,
        0XE6,0X03,0X00,0X00,0X00,0X00,0X80,0X09,0X00,0XFC,0X07,0X04,0XC0,0X03,0X00,0X00,
        0X00,0X00,0X40,0X04,0X00,0XFE,0X07,0X00,0X80,0X03,0X00,0X00,0X00,0X00,0X20,0X0A,
        0X00,0XFE,0X07,0X00,0X00,0X07,0X00,0X00,0X00,0X00,0XE0,0X09,0X00,0XFF,0X03,0XC0,
        0X01,0X07,0X00,0X00,0X00,0X00,0X00,0X04,0X00,0XFF,0X03,0X80,0X00,0X06,0X00,0X00,
        0X00,0X00,0X00,0X04,0X00,0XFE,0X03,0X80,0X03,0X0E,0X00,0X00,0X00,0X00,0X00,0X02,
        0X00,0XFC,0X03,0X40,0X00,0X07,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0XF8,0X02,0X00,
        0X80,0X07,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X00,0X08,0X00,0X60,0X00,0X00,0X00,
        0X00,0X00,0X00,0X70,0X00,0X00,0X18,0X00,0XD8,0X00,0X00,0X00,0X00,0X00,0X00,0X8D,
        0X00,0X08,0XE0,0X00,0X87,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF9,0X63,0X00,0X1B,
        0X42,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0XFF,0X00,0X04,0X94,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X80,0X1F,0X03,0X02,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X04,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF0,0X03,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

#endif //MY_FLYBACK_MYBMP_H