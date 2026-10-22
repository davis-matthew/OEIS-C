/*
 * A307081  Triangular array read by rows: row n consists of the numbers k
 *          from 1 to n sorted in ascending order according to the value of
 *          f(k) = A095112(k)/k.
 * https://oeis.org/A307081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Computes OEIS sequence A307081 by sorting each triangular row of
 *            numbers 1..r according to f(k) = Σ_{p|k} Σ_{i=1..v_p(k)} 1/p^i
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/gcd.h"

typedef struct {
    seq_t num;
    seq_t den;
} frac_t;

/* Global table of fractions f(k) for k up to MAX_K. */
static frac_t *f_table = NULL;
static idx_t  f_table_size = 0;

static void init_f_table(idx_t max_k)
{
    if (max_k <= f_table_size) return;
    free(f_table);
    f_table = (frac_t *)malloc((size_t)(max_k + 1) * sizeof(frac_t));
    f_table_size = max_k;

    for (idx_t k = 1; k <= max_k; ++k) {
        if (k == 1) {          /* f(1) = 0 */
            f_table[1].num = 0;
            f_table[1].den = 1;
            continue;
        }

        seq_t num = 0;
        seq_t den = 1;
        uint64_t n = (uint64_t)k;

        for (uint64_t p = 2; p * p <= n; ++p) {
            if (n % p == 0) {
                int cnt = 0;
                while (n % p == 0) {
                    n /= p;
                    ++cnt;
                }
                seq_t p_pow = pow_uint(p, (idx_t)cnt);
                seq_t term_num = p_pow - 1;
                seq_t term_den = (p - 1) * p_pow;

                seq_t new_num = num * term_den + term_num * den;
                seq_t new_den = den * term_den;
                seq_t g = gcd(new_num, new_den);
                num = new_num / g;
                den = new_den / g;
            }
        }
        if (n > 1) {            /* remaining prime factor */
            uint64_t p = n;
            seq_t p_pow = pow_uint(p, 1);
            seq_t term_num = p_pow - 1;
            seq_t term_den = (p - 1) * p_pow;

            seq_t new_num = num * term_den + term_num * den;
            seq_t new_den = den * term_den;
            seq_t g = gcd(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        }

        f_table[k].num = num;
        f_table[k].den = den;
    }
}

/* Find row r such that r(r+1)/2 >= n. */
static idx_t row_index(idx_t n)
{
    for (idx_t r = 1; ; ++r) {
        idx_t tri = r * (r + 1) / 2;
        if (tri >= n) return r;
    }
}

/* Comparator for sorting indices by their fractions f(k). */
static int cmp_indices(const void *a, const void *b)
{
    idx_t ia = *(const idx_t *)a;
    idx_t ib = *(const idx_t *)b;
    frac_t *fa = &f_table[ia];
    frac_t *fb = &f_table[ib];

    seq_t left  = fa->num * fb->den;
    seq_t right = fb->num * fa->den;
    if (left < right) return -1;
    if (left > right) return 1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    const idx_t MAX_K = 200;             /* sufficient for n <= 10000 */
    init_f_table(MAX_K);

    idx_t r   = row_index(n);
    idx_t prev = r * (r - 1) / 2;
    idx_t pos  = n - prev;              /* 1‑based position within the row */

    idx_t *inds = (idx_t *)malloc((size_t)r * sizeof(idx_t));
    for (idx_t i = 0; i < r; ++i) inds[i] = i + 1;

    qsort(inds, (size_t)r, sizeof(idx_t), cmp_indices);

    idx_t result = inds[pos - 1];
    free(inds);
    return (seq_t)result;
}
