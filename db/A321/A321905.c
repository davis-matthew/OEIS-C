/*
 * A321905  Irregular table read by rows: T(n,k) is the smallest m such that
 *          m^(1/m) == 2*k + 1 (mod 2^n), 0 <= k <= 2^(n-1) - 1.
 * https://oeis.org/A321905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   compute T(n,k) = smallest odd m <= 2^n-1 such that (2*k+1)^m ≡ m
 *            (mod 2^n) a(n) is the row‑major enumeration of the rows of T:
 *            a(n) = T(r,k) with r = ceil(log2(n+1)), k = n - 2^(r-1)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses mod_pow utility from the build. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static seq_t **row_cache = NULL;                    /* cached rows: index 1 .. r */
static size_t  max_cached = 0;                      /* largest r cached */

static unsigned ceil_log2_uint64(uint64_t x) {
    unsigned r = 0;
    uint64_t y = 1;
    while (y < x) {
        y <<= 1;
        ++r;
    }
    return r;
}

static void compute_row(unsigned r) {
    uint64_t row_len   = (uint64_t)1 << (r - 1);
    uint64_t mod_val   = (uint64_t)1 << r;
    seq_t   mod_seq    = (seq_t)mod_val;          /* modulus for mod_pow */

    seq_t *row = (seq_t *)malloc(row_len * sizeof(seq_t));
    if (!row) exit(1);                           /* abort on OOM */

    for (uint64_t k = 0; k < row_len; ++k) {
        uint64_t base = 1 + 2 * k;               /* 2*k + 1, odd */
        seq_t    m_val = 0;
        for (uint64_t m = 1; m < mod_val; m += 2) {
            seq_t p = mod_pow((seq_t)base, m, mod_seq);
            if (p == (seq_t)m) { m_val = (seq_t)m; break; }
        }
        row[k] = m_val;
    }
    row_cache[r] = row;
}

/*--------------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined index, return 0 as fallback */

    /* Determine row r and column k */
    uint64_t idx = (uint64_t)n;          /* treat n as 64‑bit unsigned */
    unsigned r   = ceil_log2_uint64(idx + 1);  /* row number 1‑based */
    uint64_t k   = idx - ((uint64_t)1 << (r - 1));  /* column index 0‑based */

    /* Ensure cache big enough */
    if (r > max_cached) {
        size_t new_size = r + 1;          /* we use 1‑based indices */
        seq_t **new_cache = (seq_t **)realloc(row_cache, new_size * sizeof(seq_t *));
        if (!new_cache) exit(1);
        /* Initialize new slots to NULL */
        for (size_t i = max_cached + 1; i <= r; ++i) new_cache[i] = NULL;
        row_cache = new_cache;
        max_cached = r;
    }

    /* Compute the missing row if necessary */
    if (row_cache[r] == NULL) compute_row(r);

    return row_cache[r][k];
}
