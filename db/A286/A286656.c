/*
 * A286656  Square array A(n,k), n>=0, k>=1, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} (1 - x^j)/(1 -
 *          x^(k*j)).
 * https://oeis.org/A286656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A286656: a(n) = coefficient of x^r in Π_{j≥1}(1 - x^j)/(1 - x^(c*j)),
    where r and c are obtained by reading the array by antidiagonals
    (n = r + (c-1)(c)/2,  r ≥ 0, c ≥ 1). */
#include <stdlib.h>
#include <stddef.h>

/* Use the seq_t and idx_t types supplied by the build; no redefinition.   */

/* Helper: floor(sqrt(n)) for 64‑bit unsigned integers.  This is a
    self‑contained Newton method; it is safe for any idx_t value. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n, y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Tables of pentagonal coefficients e[n] and partition numbers p[n].
   They are resized as needed. */
static seq_t *e_arr   = NULL;
static seq_t *p_arr   = NULL;
static size_t table_len = 0;

/* Ensure that e_arr and p_arr contain values up to at least max_n
   (inclusive).  The function rebuilds the entire tables from scratch
   when a larger size is requested. */
static void init_tables(idx_t max_n)
{
    size_t len = (size_t)max_n + 1;
    if (len <= table_len) return;          /* already sufficient */

    /* Free old tables if they exist. */
    if (e_arr) free(e_arr);
    if (p_arr) free(p_arr);

    e_arr = (seq_t *)malloc(len * sizeof(seq_t));
    p_arr = (seq_t *)malloc(len * sizeof(seq_t));
    table_len = len;

    /* Pentagonal coefficients e[n] (coefficients of ∏(1 - x^j)). */
    for (size_t i = 0; i < len; ++i) e_arr[i] = 0;
    e_arr[0] = 1;
    for (size_t k = 1; ; ++k) {
        idx_t p1 = k * (3 * k - 1) / 2;
        if (p1 >= len) break;
        e_arr[p1] = (k & 1) ? (seq_t)-1 : (seq_t)1;
        idx_t p2 = k * (3 * k + 1) / 2;
        if (p2 >= len) break;
        e_arr[p2] = (k & 1) ? (seq_t)-1 : (seq_t)1;
    }

    /* Unrestricted partition numbers p[n].  Classic DP. */
    for (size_t i = 0; i < len; ++i) p_arr[i] = 0;
    p_arr[0] = 1;
    for (size_t part = 1; part < len; ++part) {
        for (size_t i = part; i < len; ++i) {
            p_arr[i] += p_arr[i - part];
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Map n to diagonal index s, then to (row, column). */
    idx_t t = 8 * n + 1;
    idx_t sqrt_t = floor_isqrt_u64(t);
    idx_t s = (sqrt_t - 1) / 2 + 1;         /* s ≥ 1 */
    idx_t prev = (s - 1) * s / 2;           /* elements before diagonal s */
    idx_t r = n - prev;                     /* row (0‑based) */
    idx_t c = s - r;                        /* column (1‑based) */

    /* Ensure tables cover r. */
    init_tables(r);

    /* Compute a(r,c) = Σ_{m=0}^{⌊r/c⌋} e[r–c·m] * p[m]. */
    seq_t result = 0;
    idx_t limit = r / c;
    for (idx_t m = 0; m <= limit; ++m) {
        seq_t term = e_arr[r - c * m];
        term *= p_arr[m];
        result += term;
    }
    return result;
}
