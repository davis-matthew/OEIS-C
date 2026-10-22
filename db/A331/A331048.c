/*
 * A331048  Nearest integer to A001055(n)/A045778(n), where A001055 is
 *          factorizations and A045778 is strict factorizations.
 * https://oeis.org/A331048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt, round_half_to_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/round_half_to_even.h"

/*  A001055(n) = number of (non‑decreasing) factorizations of n
    A045778(n) = number of strict (strictly increasing) factorizations of n
    a(n)   = nearest integer to A001055(n)/A045778(n), ties rounded to even */

/* ------------------------------------------------------------ */
/*  Memoised recursive calculation of A001055(n, m)             */
/* ------------------------------------------------------------ */
#define HASH_SIZE 131071

static uint64_t f_key[HASH_SIZE];
static seq_t f_val[HASH_SIZE];
static uint8_t f_used[HASH_SIZE];

static inline seq_t f_calc(idx_t n, idx_t m)
{
    if (n == (idx_t)1)
        return (seq_t)1;               /* empty factorization */

    if (m > n)
        return (seq_t)0;               /* no factor >= m can divide n */

    /* simple hash of the (n,m) pair */
    uint64_t key = (((uint64_t)n) << 32) | (uint64_t)m;
    size_t h   = (size_t)(key % HASH_SIZE);

    /* probe in the open‑addressed table */
    while (f_used[h]) {
        if (f_key[h] == key)
            return f_val[h];
        h = (h + 1) % HASH_SIZE;
    }

    seq_t total = (seq_t)0;
    seq_t sq    = floor_sqrt((seq_t)n);
    idx_t max_d = (idx_t)sq;

    for (idx_t d = m; d <= max_d; ++d) {
        if (n % d == 0) {
            total = total + f_calc(n / d, d);
        }
    }

    /* the factorization consisting solely of [n]   */
    total = total + (seq_t)1;

    /* store result */
    f_used[h] = 1;
    f_key[h]  = key;
    f_val[h]  = total;
    return total;
}

/* ------------------------------------------------------------ */
/*  Memoised recursive calculation of A045778(n, m)             */
/* ------------------------------------------------------------ */
static uint64_t g_key[HASH_SIZE];
static seq_t g_val[HASH_SIZE];
static uint8_t g_used[HASH_SIZE];

static inline seq_t g_calc(idx_t n, idx_t m)
{
    if (n == (idx_t)1)
        return (seq_t)1;               /* empty factorization */

    if (m > n)
        return (seq_t)0;               /* cannot start with a factor >= m */

    uint64_t key = (((uint64_t)n) << 32) | (uint64_t)m;
    size_t h = (size_t)(key % HASH_SIZE);

    while (g_used[h]) {
        if (g_key[h] == key)
            return g_val[h];
        h = (h + 1) % HASH_SIZE;
    }

    seq_t total = (seq_t)0;
    seq_t sq    = floor_sqrt((seq_t)n);
    idx_t max_d = (idx_t)sq;

    for (idx_t d = m; d <= max_d; ++d) {
        if (n % d == 0) {
            /* Next factor must be > d to keep the factors strictly increasing */
            total = total + g_calc(n / d, d + 1);
        }
    }

    /* the factorization consisting solely of [n]     */
    total = total + (seq_t)1;

    g_used[h] = 1;
    g_key[h]  = key;
    g_val[h]  = total;
    return total;
}

/* ------------------------------------------------------------ */
/*  Rounding of a rational number to the nearest integer      */
/*  with ties rounded to even                                 */

/* ------------------------------------------------------------ */
/*  Public function a(n)                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    seq_t f = f_calc(n, (idx_t)2);
    seq_t g = g_calc(n, (idx_t)2);

    return round_half_to_even(f, g);
}
