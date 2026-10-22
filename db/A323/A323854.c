/*
 * A323854  Triangle read by rows: T(n,k) is the numerator of the generalized
 *          harmonic number H(n,k) of rank k (n >= 1, 0 <= k <= n - 1).
 * https://oeis.org/A323854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) is the numerator of H(r,k) where (r,k) is the row and
 *            column corresponding to the triangular index n. H(r,k) is
 *            defined recursively by H(1,0) = 1 H(r,0) = H(r-1,0) + 1/r H(r,k)
 *            = sum_{i=1}^{r-k} H(r-i,k-1) / i for k>0
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   decode_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/decode_idx.h"

typedef struct { seq_t num; seq_t den; } Frac;

/* Euclidean algorithm for seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Reduce a fraction to lowest terms */
static Frac reduce_frac(const Frac *f)
{
    if (f->num == 0) return (Frac){0,1};
    seq_t g = gcd_seq(f->num, f->den);
    return (Frac){f->num / g, f->den / g};
}

/* Add two fractions; result is reduced */
static Frac add_frac(const Frac *a, const Frac *b)
{
    if (a->num == 0) return *b;
    if (b->num == 0) return *a;

    seq_t g = gcd_seq(a->den, b->den);
    seq_t lcm = (a->den / g) * b->den;

    seq_t mul_a = b->den / g;
    seq_t mul_b = a->den / g;

    Frac sum;
    sum.num = a->num * mul_a + b->num * mul_b;
    sum.den = lcm;
    return reduce_frac(&sum);
}

/* Triangle storage: table[1]..table[rows] each pointing to an array of Frac of size r */
static Frac **table = NULL;
static idx_t rows = 0;

/* Expand table up to 'need' rows (1‑based). Data is filled as rows are added. */
static void ensure_rows(idx_t need)
{
    if (need <= rows) return;

    Frac **new_table = rows == 0 ?
        (Frac**)malloc(sizeof(Frac*) * (need + 1)) :
        (Frac**)realloc(table, sizeof(Frac*) * (need + 1));
    if (!new_table) return;            /* allocation failed – state unchanged */

    /* Allocate and compute new rows */
    for (idx_t r = rows + 1; r <= need; ++r) {
        new_table[r] = (Frac*)malloc(sizeof(Frac) * r);   /* indices 0..r-1 */
        if (!new_table[r]) continue;    /* allocation failure – skip this row */

        /* H(r,0) */
        if (r == 1) {
            new_table[r][0] = (Frac){1, 1};
        } else {
            Frac prev = new_table[r-1][0];
            Frac term = {1, (seq_t)r};
            new_table[r][0] = add_frac(&prev, &term);
        }

        /* H(r,k) for k>0 */
        for (idx_t k = 1; k < r; ++k) {
            Frac sum = {0, 1};
            for (idx_t i = 1; i <= r - k; ++i) {
                Frac sub = new_table[r - i][k - 1];
                Frac term = {sub.num, sub.den * (seq_t)i};
                sum = add_frac(&sum, &term);
            }
            new_table[r][k] = sum;
        }
    }

    table = new_table;
    rows = need;
}

/* Required external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, harmless */

    idx_t r, k;
    decode_idx(n, &r, &k);
    ensure_rows(r);

    if (r > rows || k >= r || !table[r]) return 0;   /* safety */
    return table[r][k].num;
}
