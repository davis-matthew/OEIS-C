/*
 * A271355  Triangular array: T(n,k) = |round((r^n)*(s^k))|, where r = golden
 *          ratio = (1+sqrt(5))/2, s = (1-sqrt(5))/2, 1 <= k <= n, n >= 1.
 * https://oeis.org/A271355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) (n≥1) is the entry at row r, column k of the triangular
 *            array T where T(n,k) = A169985(n‑k+1). The row r is the smallest
 *            integer with r(r+1)/2 ≥ n. Then a(n) = A169985(r – (n – (r‑1)r/2
 *            – 1))
 * Range:     u8 n <= 78, u16 n <= 300, u32 n <= 1128, u64 n <= 4371,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*----------------------------------------------------------------------
  Index of square root for an unsigned 64‑bit integer.
  Uses only integer arithmetic (no <math.h>).
----------------------------------------------------------------------*/
static idx_t isqrt_u64(idx_t n)
{
    __uint128_t r = 0;
    __uint128_t bit = (__uint128_t)1 << 62;   /* 2^62 is the largest power of 4 ≤ 2^64 */
    while (bit) {
        __uint128_t next = r | bit;
        if (next * next <= n)
            r = next;
        bit >>= 1;
    }
    return (idx_t)r;
}

/*----------------------------------------------------------------------
  Cache of Lucas numbers L0 = 2, L1 = 1, L_n = L_{n‑1} + L_{n‑2}.
  The cache is extended on demand up to the requested index.
----------------------------------------------------------------------*/
static seq_t *lucas_cache = NULL;  /* array of Lucas numbers   */
static idx_t lucas_len   = 0;       /* number of values stored   */

static void ensure_lucas(idx_t upto)
{
    if (lucas_len > upto)
        return;                           /* already have enough      */

    seq_t *old = lucas_cache;
    lucas_cache = (seq_t *)realloc(old, sizeof(seq_t) * (upto + 1));
    if (!lucas_cache) {                 /* allocation failed         */
        free(old);
        abort();
    }

    if (lucas_len == 0) {
        /* initialise first two Lucas numbers */
        lucas_cache[0] = (seq_t)2;      /* L0 */
        lucas_cache[1] = (seq_t)1;      /* L1 */
        lucas_len = 2;
    }

    for (idx_t i = lucas_len; i <= upto; ++i) {
        lucas_cache[i] = lucas_cache[i - 1] + lucas_cache[i - 2];
    }
    lucas_len = upto + 1;
}

static seq_t lucas_value(idx_t idx)
{
    if (idx == 0) return (seq_t)2;
    if (idx == 1) return (seq_t)1;
    ensure_lucas(idx);
    return lucas_cache[idx];
}

/*----------------------------------------------------------------------
  Main sequence function
----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Determine the row r that contains the n‑th term.  r satisfies
       r(r+1)/2 ≥ n and (r‑1)r/2 < n. */
    idx_t r = (isqrt_u64(1 + 8 * n) - 1) / 2;
    /* Correct possible off‑by‑one due to floor truncation. */
    if ((((__uint128_t)r * (r + 1)) / 2) < n)
        ++r;

    /* Number of terms before this row. */
    idx_t prev = ((__uint128_t)r * (r - 1)) / 2;
    /* Zero‑based offset within the row. */
    idx_t offset = n - prev - 1;        /* 0 ≤ offset < r             */
    /* Index (1‑based) into A169985 for the current term. */
    idx_t m = r - offset;               /* 1 ≤ m ≤ r                 */

    if (m == 1)           return (seq_t)1;
    if (m == 2)           return (seq_t)2;
    /* For m ≥ 3, A169985(m) = Lucas number L_{m‑1}. */
    return lucas_value(m - 1);
}
