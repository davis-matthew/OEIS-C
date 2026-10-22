/*
 * A337127  Table with 10 columns read by rows: T(n, k) is the number of
 *          n-digit positive integers with exactly k distinct base 10 digits
 *          (0 < k <= 10).
 * https://oeis.org/A337127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A337127: read table T(n,k) row‑wise
    T(n,k) = 9 * Pochhammer(11-k,k-1) * S2(n,k)  for 1 <= k <= 10
    where S2(n,k) is the Stirling number of the 2nd kind.
    a(n) = T(row,k) where row = floor((n-1)/10)+1 and k = (n-1)%10+1
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* forward declaration of user supplied types */
extern seq_t factorial(idx_t);
extern seq_t binomial(idx_t, idx_t);

/* implementation */

static seq_t *s2_table = NULL;          /* flat array (n+1)*11 */
static idx_t s2_max_n   = 0;            /* highest n computed */
static idx_t s2_cap     = 0;            /* allocated capacity (n+1) */

static seq_t poch[11] = {0};            /* Pochhammer(11-k,k-1), 1..10 */
static bool poch_init = false;

/* ensure that S2 values are computed up to n */
static void ensure_s2(idx_t n)
{
    if (!s2_table) {                     /* first call */
        s2_cap = n + 1;
        s2_table = (seq_t *)calloc((size_t)s2_cap * 11, sizeof(seq_t));
        if (!s2_table) exit(EXIT_FAILURE);
        s2_table[0 * 11 + 0] = 1;         /* S2(0,0) = 1 */
        s2_max_n = 0;
    } else if (n >= s2_cap) {            /* need to grow */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(s2_table, (size_t)new_cap * 11 * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        s2_table = tmp;
        for (idx_t i = s2_cap; i < new_cap; ++i)
            for (int k = 0; k <= 10; ++k)
                s2_table[i * 11 + k] = 0;
        s2_cap = new_cap;
    }

    /* compute missing rows */
    for (idx_t r = s2_max_n + 1; r <= n; ++r) {
        s2_table[r * 11 + 0] = 0;
        for (int k = 1; k <= 10; ++k)
            s2_table[r * 11 + k] =
                s2_table[(r - 1) * 11 + (k - 1)] + (seq_t)k * s2_table[(r - 1) * 11 + k];
    }
    s2_max_n = n;
}

/* initialize Pochhammer table */
static void init_poch(void)
{
    if (poch_init) return;
    poch_init = true;
    poch[1] = 1; /* empty product */
    for (int k = 2; k <= 10; ++k) {
        seq_t prod = 1;
        int start = 11 - k;
        for (int t = 0; t < k - 1; ++t)
            prod *= (seq_t)(start + t);
        poch[k] = prod;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; n<1 is undefined */
    init_poch();

    idx_t idx   = n - 1;
    int    k    = (int)(idx % 10) + 1;          /* 1 .. 10 */
    idx_t row   = idx / 10 + 1;                 /* 1 .. */

    if (k > 10) return 0;

    ensure_s2(row);

    seq_t sk = s2_table[row * 11 + k];
    seq_t res = 9 * poch[k] * sk;
    return res;
}
