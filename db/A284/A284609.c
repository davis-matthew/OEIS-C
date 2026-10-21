/*
 * A284609  Number of partitions of n such that the (sum of all odd parts) =
 *          floor(n/2).
 * https://oeis.org/A284609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if ceil(n/2) is odd, otherwise a(n) = p_odd(floor(n/2))
 *            * p(ceil(n/2)/2) where p_k(m) is the number of partitions of m
 *            into parts of the specified type (any part or odd part)
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_total = NULL;   /* partitions of m (any parts) */
static size_t total_cap = 0;    /* current array size */

static seq_t *p_odd = NULL;    /* partitions of m using only odd parts */
static size_t odd_cap = 0;     /* current array size */

/* Build partition table up to n (inclusive) for all parts. */
static void ensure_total(idx_t n)
{
    if (total_cap > n) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)realloc(p_total, new_cap * sizeof(seq_t));

    /* Re‑compute from scratch. */
    new_arr[0] = 1;
    for (size_t i = 1; i < new_cap; ++i) new_arr[i] = 0;

    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            new_arr[sum] += new_arr[sum - part];
        }
    }

    p_total = new_arr;
    total_cap = new_cap;
}

/* Build partition table up to n (inclusive) for odd parts only. */
static void ensure_odd(idx_t n)
{
    if (odd_cap > n) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)realloc(p_odd, new_cap * sizeof(seq_t));

    /* Re‑compute from scratch. */
    new_arr[0] = 1;
    for (size_t i = 1; i < new_cap; ++i) new_arr[i] = 0;

    for (idx_t part = 1; part <= n; part += 2) {
        for (idx_t sum = part; sum <= n; ++sum) {
            new_arr[sum] += new_arr[sum - part];
        }
    }

    p_odd = new_arr;
    odd_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t s = n / 2;          /* floor(n/2) – sum of odd parts */
    idx_t t = n - s;          /* ceil(n/2) – sum of even parts */

    /* Even parts sum must be even. */
    if (t & 1) return 0;
    idx_t u = t / 2;          /* (ceil(n/2))/2 */

    ensure_total(u);
    ensure_odd(s);

    return p_total[u] * p_odd[s];
}
