/*
 * A323555  Irregular table read by rows: T(n,k) = (2*k+1)^(1/5) mod 2^n, 0 <=
 *          k <= 2^(n-1) - 1.
 * https://oeis.org/A323555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r = ⌊log₂(n)⌋+1 and k = n-2^(r-1); T(r,k) is
 *            the unique odd x modulo 2^r with x^5 ≡ 2k+1 (mod 2^r)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* forward declarations */
static inline idx_t floor_log2_u64(idx_t v);
static void generate_row(idx_t r);

/* static tables for cached rows (1‑based indexing, up to r = 64) */
static seq_t *row_tables[65] = {0};

static inline idx_t floor_log2_u64(idx_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(v);
#else
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

/* generate the permutation array for row r (1 ≤ r) */
static void generate_row(idx_t r)
{
    /* number of entries in this row is 2^(r-1) */
    idx_t size = (idx_t)1 << (r - 1);
    seq_t *arr = (seq_t *)malloc(size * sizeof(seq_t));
    assert(arr != NULL);
    /* mask for modulus 2^r */
    seq_t mask = ((seq_t)1 << r) - 1;

    for (seq_t x = 1; x <= mask; x += 2) {
        /* compute y = x^5 mod 2^r */
        seq_t a = (x * x) & mask;          /* x²   */
        seq_t b = (a * a) & mask;          /* x⁴   */
        seq_t y = (b * x) & mask;          /* x⁵   */
        /* k = (y-1)/2  (0‑based index) */
        idx_t k = (idx_t)((y - 1) >> 1);
        arr[k] = x;
    }

    row_tables[r] = arr;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */

    /* determine row r and intra‑row index k */
    idx_t r = floor_log2_u64(n) + 1;
    assert(r <= 64);               /* safe for n up to 10 000 */

    if (row_tables[r] == NULL)
        generate_row(r);

    idx_t k = n - ((idx_t)1 << (r - 1));   /* 0‑based index within the row */
    return row_tables[r][k];
}
