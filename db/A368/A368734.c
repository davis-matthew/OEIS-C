/*
 * A368734  Four-column table read by rows where row n lists the entries of
 *          the 2 X 2 matrix M(n) used to form Bird tree and Drib tree
 *          rationals.
 * https://oeis.org/A368734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the row‑by‑row listing of the four entries (x(n), y(n),
 *            z(n), t(n)) of the 2×2 matrix M(n) defined by the recurrences:
 *            f(1) = (1,0), g(1) = (0,1); f(2k) = g(k); f(2k+1) = f(k)+g(k);
 *            g(2k) = f(k)+g(k); g(2k+1) = f(k);
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *f_x = NULL;
static seq_t *f_y = NULL;
static seq_t *g_z = NULL;
static seq_t *g_t = NULL;
static size_t capacity = 0;
static size_t max_computed = 0;

static void grow_capacity(size_t min_cap)
{
    if (capacity >= min_cap) return;
    size_t new_cap = capacity ? capacity << 1 : 1;
    while (new_cap < min_cap) new_cap <<= 1;
    f_x = (seq_t*)realloc(f_x, (new_cap + 1) * sizeof(seq_t));
    f_y = (seq_t*)realloc(f_y, (new_cap + 1) * sizeof(seq_t));
    g_z = (seq_t*)realloc(g_z, (new_cap + 1) * sizeof(seq_t));
    g_t = (seq_t*)realloc(g_t, (new_cap + 1) * sizeof(seq_t));
    capacity = new_cap;
}

static void compute_up_to(size_t n)
{
    if (n <= max_computed) return;
    grow_capacity(n);
    for (size_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            f_x[1] = (seq_t)1;
            f_y[1] = (seq_t)0;
            g_z[1] = (seq_t)0;
            g_t[1] = (seq_t)1;
        } else {
            size_t k = i >> 1;          /* floor(i/2) */
            if ((i & 1) == 0) {         /* even : i = 2k */
                f_x[i] = g_z[k];        /* f(i) = g(k) */
                f_y[i] = g_t[k];
                g_z[i] = f_x[k] + g_z[k]; /* g(i) = f(k)+g(k) */
                g_t[i] = f_y[k] + g_t[k];
            } else {                    /* odd : i = 2k+1 */
                f_x[i] = f_x[k] + g_z[k]; /* f(i) = f(k)+g(k) */
                f_y[i] = f_y[k] + g_t[k];
                g_z[i] = f_x[k];          /* g(i) = f(k) */
                g_t[i] = f_y[k];
            }
        }
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;    /* undefined for n < 1 */
    size_t row = (size_t)((n - (idx_t)1) / 4 + (idx_t)1);
    unsigned pos = (unsigned)((n - (idx_t)1) % 4);
    compute_up_to(row);
    switch (pos) {
        case 0: return f_x[row];
        case 1: return f_y[row];
        case 2: return g_z[row];
        case 3: return g_t[row];
        default: return (seq_t)0;  /* unreachable */
    }
}
