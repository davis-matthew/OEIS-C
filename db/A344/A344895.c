/*
 * A344895  a(n) is the number of letters in the official romanization of the
 *          Bulgarian name of n, excluding spaces.
 * https://oeis.org/A344895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters in the official romanization of the
 *            Bulgarian name of n, excluding spaces. n ranges over all
 *            non‑negative integers. The function implements the Bulgarian
 *            number‑word rules with up‑to‑billion support
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Lengths of unit words 0 – 19 */
static const unsigned char unit_len[20] = {
    4, /* 0 nula */
    4, /* 1 edno */
    3, /* 2 dva */
    3, /* 3 tri */
    7, /* 4 chetiri */
    3, /* 5 pet */
    5, /* 6 shest */
    5, /* 7 sedem */
    4, /* 8 osam */
    5, /* 9 devet */
    5, /*10 deset */
   10, /*11 edinadeset */
   10, /*12 dvanaest */
   10, /*13 trinadeset */
   14, /*14 chetirnadeset */
   10, /*15 petnadest */
   12, /*16 shestnadest */
   12, /*17 sedemnadeset */
   11, /*18 osmnadeset */
   12  /*19 devetnadeset */
};

/* Lengths of tens 20 – 90 */
static const unsigned char tens_len[10] = {
    0, /* dummy 0 */
    0, /* dummy 1 */
    8, /* 20 dvadeset   */
    8, /* 30 trideset   */
    12,/* 40 chetirideset */
    8, /* 50 petdeset    */
    10,/* 60 shestdeset  */
    10,/* 70 sedemdeset  */
    9, /* 80 osemdeset   */
    10/* 90 devetdeset  */
};

/* Lengths of hundreds 100 – 900 */
static const unsigned char hundred_len[10] = {
    0, /* dummy 0 */
    3, /* 100 sto         */
    6, /* 200 dvesti      */
    5, /* 300 trista       */
    13,/* 400 chetiristotin*/
    9, /* 500 petstotin    */
    11,/* 600 sheststotin  */
    11,/* 700 sedemstotin  */
    10,/* 800 osemstotin   */
    11 /* 900 devetstotin  */
};

/* Recursive helper that returns the number of letters of n,
   excluding spaces, for any non‑negative integer n. */
static seq_t len_num(idx_t n)
{
    /* 0 – 19 */
    if (n < 20) {
        return (seq_t)unit_len[(int)n];
    }

    /* 20 – 99 */
    if (n < 100) {
        unsigned int ten = (unsigned int)(n / 10);
        unsigned int rem = (unsigned int)(n % 10);
        seq_t res = (seq_t)tens_len[ten];
        if (rem) {
            res += 1 + (seq_t)unit_len[rem];  /* 'i' + unit word */
        }
        return res;
    }

    /* 100 – 999 */
    if (n < 1000) {
        unsigned int hund = (unsigned int)(n / 100);
        unsigned int rem = (unsigned int)(n % 100);
        seq_t res = (seq_t)hundred_len[hund];
        if (rem) {
            res += 1 + len_num(rem);          /* 'i' + remainder */
        }
        return res;
    }

    /* 1 000 – 999 999 : thousands */
    if (n < 1000000ULL) {
        idx_t th = n / 1000;
        idx_t rem = n % 1000;
        seq_t res;
        if (th == 1) {
            res = 7;                          /* "hiliad" */
        } else {
            res = len_num(th) + 7;            /* unit + "hiliad" */
        }
        if (rem) {
            res += 1 + len_num(rem);          /* 'i' + remainder */
        }
        return res;
    }

    /* 1 000 000 – 999 999 999 : millions */
    if (n < 1000000000ULL) {
        idx_t mil = n / 1000000;
        idx_t rem = n % 1000000;
        seq_t res;
        if (mil == 1) {
            res = 6;                          /* "milion" */
        } else {
            res = len_num(mil) + 6;           /* unit + "milion" */
        }
        if (rem) {
            res += 1 + len_num(rem);          /* 'i' + remainder */
        }
        return res;
    }

    /* 1 000 000 000 – 999 999 999 999 : billions */
    if (n < 1000000000000ULL) {
        idx_t bil = n / 1000000000;
        idx_t rem = n % 1000000000;
        seq_t res;
        if (bil == 1) {
            res = 7;                          /* "miliard" */
        } else {
            res = len_num(bil) + 7;           /* unit + "miliard" */
        }
        if (rem) {
            res += 1 + len_num(rem);          /* 'i' + remainder */
        }
        return res;
    }

    /* For larger numbers, recursively apply the thousand grouping. */
    idx_t group = n / 1000;
    idx_t rem = n % 1000;
    seq_t res = len_num(group) + 7;               /* treat each group as nestled "hiliad" */
    if (rem) {
        res += 1 + len_num(rem);
    }
    return res;
}

/* Main entry point required by the problem statement. */
seq_t a(idx_t n)
{
    return len_num(n);
}
