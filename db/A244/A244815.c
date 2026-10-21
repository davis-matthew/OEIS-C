/*
 * A244815  The hexagonal spiral of Champernowne, read along the 210-degree
 *          ray.
 * https://oeis.org/A244815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = digit at position (3*n^2 - 4*n + 2) in Champernowne's constant
 * (concatenation of the natural numbers 1,2,3,…)
 */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑computed powers of ten up to 10^18 */
static uint64_t pow10_table[20];
static bool pow10_inited = false;

static void init_pow10(void)
{
    if (pow10_inited) return;
    pow10_table[0] = 1ULL;
    for (int i = 1; i < 20; ++i) {
        pow10_table[i] = pow10_table[i-1] * 10ULL;
    }
    pow10_inited = true;
}

/* Returns the digit (0‑9) at the given 1‑based position in Champernowne's constant */
static seq_t champernowne_digit(idx_t pos)
{
    /* Make position 0‑based for easier calculations */
    uint64_t p = pos - 1ULL;

    /* Find the block of numbers that contains the digit */
    for (int len = 1; ; ++len) {
        uint64_t count = 9ULL * pow10_table[len-1];          /* number of len‑digit numbers */
        __uint128_t block_digits = (__uint128_t)len * count; /* digits contributed by that block */

        if (p < (uint64_t)block_digits) {
            /* The digit lies in this block */
            uint64_t offset = p / (uint64_t)len;           /* 0‑based index of the number within the block */
            uint64_t number  = pow10_table[len-1] + offset;

            int digit_pos = (int)(p % (uint64_t)len);       /* 0‑based digit index from the left */
            uint64_t factor = pow10_table[len - digit_pos - 1];
            seq_t digit = (seq_t)((number / factor) % 10ULL);
            return digit;
        }
        /* Skip the entire block and continue */
        p -= (uint64_t)block_digits;
    }
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    init_pow10();

    /* Compute k = 3*n^2 - 4*n + 2, using 128‑bit arithmetic to avoid overflow */
    __uint128_t tn = n;
    __uint128_t k128 = 3ULL * tn * tn - 4ULL * tn + 2ULL;
    /* Convert back to uint64_t; assume the result fits into 64 bits for all used n */
    idx_t k = (idx_t)k128;

    /* The Champernowne digit is the answer */
    return champernowne_digit(k);
}
