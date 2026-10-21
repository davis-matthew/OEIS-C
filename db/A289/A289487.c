/*
 * A289487  Decimal expansion of 4/h, where h is the Planck constant in SI
 *          units.
 * https://oeis.org/A289487
 *
 * Offset:    34    (a(34) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth decimal digit of 4/h, with offset 34. That is,
 *            a(34) is the most significant decimal digit of the integer part
 *            of 4/h, and thereafter the remaining digits of the integer part
 *            followed by the fractional digits
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint32_t DEN = 662607015U;          /* denominator of 4/h */
static const uint64_t BASE = 1000000000ULL;      /* base for big integer limbs */

/* State for the sequence */
static int initialized = 0;
static int int_len = 0;                        /* number of integer digits */
static seq_t int_digits[64];                   /* holds integer part digits */

static seq_t rem = 0;                          /* remainder after integer part */
static seq_t *frac_digits = NULL;              /* fractional digits cache */
static size_t frac_cap   = 0;                  /* capacity of frac_digits */
static size_t frac_len   = 0;                  /* number of produced fractional digits */

/* ---------- auxiliary functions for big integer division ---------- */
static void compute_int_and_remainder(void)
{
    /* Represent 4*10^42 as big integer in base 1e9: [0,0,0,0,4000000] */
    uint32_t big[5] = {0, 0, 0, 0, 4000000U};
    uint32_t qlimb[5] = {0};
    uint32_t r_small = 0;           /* remainder modulo DEN */

    /* Divide big by DEN, producing quotient limbs in qlimb */
    for (int i = 4; i >= 0; --i) {
        unsigned __int128 cur = (unsigned __int128)r_small * BASE + big[i];
        qlimb[i] = (uint32_t)(cur / DEN);
        r_small  = (uint32_t)(cur % DEN);
    }
    rem = (seq_t)r_small;          /* remainder after integer part */

    /* Convert quotient limbs (qlimb) to decimal digits */
    /* Copy qlimb to temp array for conversion */
    uint32_t temp[5];
    int temp_len = 0;
    for (int i = 0; i < 5; ++i) {
        temp[i] = qlimb[i];
        if (i > temp_len && qlimb[i] != 0)
            temp_len = i;
    }
    /* temp_len is the highest non-zero limb index */
    temp_len += 1;

    /* Extract decimal digits (reverse order) */
    seq_t rev_digits[70];      /* enough for 34 digits */
    int rev_cnt = 0;
    while (temp_len > 0) {
        unsigned __int128 cur = 0;
        uint32_t rem10 = 0;
        for (int i = temp_len - 1; i >= 0; --i) {
            cur = (unsigned __int128)rem10 * BASE + temp[i];
            temp[i] = (uint32_t)(cur / 10);
            rem10   = (uint32_t)(cur % 10);
        }
        rev_digits[rev_cnt++] = (seq_t)rem10;
        /* remove leading zero limbs */
        while (temp_len > 0 && temp[temp_len - 1] == 0)
            temp_len--;
    }

    /* Reverse digits to correct order */
    int_len = rev_cnt;
    for (int i = 0; i < rev_cnt; ++i)
        int_digits[i] = rev_digits[rev_cnt - 1 - i];
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 34)
        return (seq_t)0;          /* undefined for n < offset, return 0 */

    if (!initialized) {
        compute_int_and_remainder();
        initialized = 1;
    }

    /* Index of first integer digit */
    const idx_t int_start = 34;

    if (n < int_start + int_len) {
        /* Return digit from integer part */
        return int_digits[(size_t)(n - int_start)];
    }

    /* Fractional part */
    idx_t target = n - (int_start + int_len);   /* 0‑based index into frac_digits */

    /* Grow cache if needed */
    while (frac_len <= (size_t)target) {
        /* compute next fractional digit using remainder */
        unsigned __int128 prod = (unsigned __int128)rem * 10;
        seq_t digit = (seq_t)(prod / DEN);
        rem = (seq_t)(prod % DEN);

        /* store digit */
        if (frac_len >= frac_cap) {
            size_t new_cap = frac_cap ? frac_cap * 2 : 1024;
            seq_t *new_arr = (seq_t *)realloc(frac_digits, new_cap * sizeof(seq_t));
            if (!new_arr) abort();   /* out of memory */
            frac_digits = new_arr;
            frac_cap = new_cap;
        }
        frac_digits[frac_len++] = digit;
    }

    return frac_digits[(size_t)target];
}
