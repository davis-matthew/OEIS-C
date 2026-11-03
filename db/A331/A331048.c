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
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ------------------------------------------------------------ */
static inline seq_t round_half_to_even(seq_t num, seq_t den)
{
    seq_t q = num / den;          /* integer part  */
    seq_t r = num % den;          /* remainder     */
    seq_t twice = r + r;          /* r * 2         */

    if (twice < den)
        return q;                 /* round down    */
    if (twice > den)
        return q + 1;             /* round up      */

    /* tie: choose the even integer */
    if ((q & (seq_t)1) == 0)
        return q;
    else
        return q + 1;
}

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
