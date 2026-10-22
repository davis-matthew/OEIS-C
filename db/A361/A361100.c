/*
 * A361100  Decimal expansion of 2^(2^(2^(2^2))) = 2^^5.
 * https://oeis.org/A361100
 *
 * Offset:    19730    (a(19730) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = the (n-19730)th decimal digit of 2^65536 (0‑based)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts at offset 19730. For n < 19730 or beyond the length, returns 0. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* Constants for 2^65536 */
#define EXP_BIT   65536U       /* exponent of 2 */
#define WORD_BITS 32           /* word size for binary array (uint32_t) */
#define WORDS     2049         /* number of 32‑bit words needed (ceil((65536+1)/32)) */

/* Static state, initialized on first call */
static bool seq_ready = false;
static char *seq_digits = NULL;   /* decimal digits string */
static size_t seq_len = 0;        /* number of digits */

/* ------------------------------------------------------------ */
/* Initialize the decimal string representation of 2^65536.     */
/* ------------------------------------------------------------ */
static void init_seq_digits(void)
{
    /* Binary representation in 32‑bit words, little endian */
    uint32_t words[WORDS];
    memset(words, 0, sizeof(words));

    /* Set bit 65536 */
    size_t bit = EXP_BIT;
    size_t word_idx = bit / WORD_BITS;      /* 2048 */
    unsigned bit_offset = bit % WORD_BITS;  /* 0 */
    words[word_idx] = 1U << bit_offset;

    /* Divide by 1e9 repeatedly to obtain base‑1e9 digits (least significant first) */
    const uint32_t BASE = 1000000000U;
    uint32_t base_digits[2200]; /* enough for ceil(19730/9)+1 = 2193 */
    size_t bd_cnt = 0;

    size_t top = WORDS - 1;
    while (top > 0 && words[top] == 0) top--;

    while (top > 0 || words[0] != 0) {
        uint64_t rem = 0;
        for (size_t i = top + 1; i > 0; --i) {
            size_t idx = i - 1;
            uint64_t cur = ((uint64_t)rem << WORD_BITS) | (uint64_t)words[idx];
            words[idx] = (uint32_t)(cur / BASE);
            rem = cur % BASE;
        }
        base_digits[bd_cnt++] = (uint32_t)rem;

        while (top > 0 && words[top] == 0) top--;
    }

    /* Convert base-1e9 digits to a plain decimal string */
    uint32_t ms = base_digits[bd_cnt - 1];   /* most significant chunk */
    int ms_len;
    if (ms >= 100000000U) ms_len = 9;
    else if (ms >= 10000000U) ms_len = 8;
    else if (ms >= 1000000U) ms_len = 7;
    else if (ms >= 100000U) ms_len = 6;
    else if (ms >= 10000U) ms_len = 5;
    else if (ms >= 1000U) ms_len = 4;
    else if (ms >= 100U) ms_len = 3;
    else if (ms >= 10U) ms_len = 2;
    else ms_len = 1;

    size_t total_len = ms_len + 9 * (bd_cnt - 1);
    char *str = (char *)malloc(total_len + 1);
    if (!str) exit(1); /* allocation failed */

    size_t pos = 0;
    char buf[16];

    /* most significant part */
    int len = snprintf(buf, sizeof(buf), "%u", ms);
    memcpy(str + pos, buf, len);
    pos += len;

    /* remaining parts with leading zeros to 9 digits */
    for (int i = (int)bd_cnt - 2; i >= 0; --i) {
        snprintf(buf, sizeof(buf), "%09u", base_digits[i]);
        memcpy(str + pos, buf, 9);
        pos += 9;
    }
    str[total_len] = '\0';

    seq_digits = str;
    seq_len = total_len;
    seq_ready = true;
}

/* ------------------------------------------------------------ */
/* Public sequence function                                   */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!seq_ready) init_seq_digits();

    /* Offset of the first term */
    static const idx_t OFFSET = 19730;

    if (n < OFFSET || n >= OFFSET + seq_len)
        return (seq_t)0; /* undefined for out of range */

    size_t idx = (size_t)(n - OFFSET);
    return (seq_t)(seq_digits[idx] - '0'); /* digit to integer */
}
