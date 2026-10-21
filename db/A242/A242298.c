/*
 * A242298  Once a number in this sequence is divisible by all numbers 1 to m,
 *          subsequent terms are constrained to have the same property; choose
 *          the smallest permissible number that is greater than the previous
 *          term.
 * https://oeis.org/A242298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1, a(n) = a(n-1) + L, where L is the current
 *            least common multiple of the integers 1 … m, and m is the
 *            maximum integer such that a(n-1) is divisible by all 1 … m.
 *            After each addition, m and L are updated while the new term
 *            remains divisible by m+1, m+2, …
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 92, u64 n <= 280,
 *            u128 n <= 914, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;     /* cache of computed terms, 1‑based */
static size_t cap = 0;        /* allocated capacity of seq array   */
static idx_t  max_n = 0;      /* highest index currently stored   */
static idx_t  cur_m = 1;      /* current maximal m                 */
static seq_t  cur_lcm = 1;    /* L = lcm(1…cur_m)                   */

static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cap) return;
    size_t new_cap = cap ? cap << 1 : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    seq = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is undefined for n < 1 */

    if (max_n == 0) {             /* first call: initialise a(1) */
        ensure_capacity(1);
        seq[1] = (seq_t)1;
        max_n = 1;
        cur_m   = 1;
        cur_lcm = (seq_t)1;
    }

    if (n <= max_n)               /* already computed */
        return seq[n];

    ensure_capacity(n);

    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t prev = seq[i-1];
        seq_t next = prev + cur_lcm;   /* candidate term          */

        /* update m and lcm while new term is divisible by the next integer */
        idx_t m   = cur_m;
        seq_t l   = cur_lcm;
        idx_t k   = m + 1;

        while ((next % (seq_t)k) == 0) {
            m = k;
            seq_t g = gcd(l, (seq_t)k);
            l = (l / g) * (seq_t)k; /* update lcm(1…k)           */
            ++k;
        }

        cur_m   = m;
        cur_lcm = l;
        seq[i]  = next;
    }

    max_n = n;
    return seq[n];
}
