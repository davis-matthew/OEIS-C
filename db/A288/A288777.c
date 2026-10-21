/*
 * A288777  Triangle read by rows in which column k lists the positive
 *          multiples of the factorial of k, with 1 <= k <= n.
 * https://oeis.org/A288777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (n‑th) element read by rows of the triangle T(r,k) =
 *            (r - k + 1) * k! (1 <= k <= r). Row r contains the k‑values k =
 *            1 .. r. For a given index n we find the minimal r such that
 *            r(r+1)/2 >= n, then k = n - r(r-1)/2, and compute a(n) = (r - k
 *            + 1) * k!
 * Range:     u8 n <= 20, u16 n <= 43, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: find the row r that contains index n (n >= 1) */
static idx_t row_of(idx_t n) {
    idx_t low = 1;
    idx_t high = n;          /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use 128‑bit to avoid overflow on the product */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Cache factorial values */
static seq_t *fact = NULL;     /* array of factorials */
static size_t fact_cap = 0;    /* allocated capacity of fact array */
static size_t fact_len = 0;    /* currently stored factorials, fact_len-1 is last */

static void ensure_factorial(idx_t k) {
    if (fact_len > k)
        return;
    /* Grow the array */
    size_t needed = (size_t)k + 1;
    if (fact_cap < needed) {
        size_t new_cap = fact_cap ? fact_cap * 2 : 8;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – give up */
            exit(EXIT_FAILURE);
        }
        fact = tmp;
        fact_cap = new_cap;
    }
    if (fact_len == 0) {
        fact[0] = (seq_t)1;
        fact_len = 1;
    }
    for (idx_t i = fact_len; i <= k; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_len = (size_t)k + 1;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, return 0 */
    idx_t r = row_of(n);
    /* compute prior terms count: r*(r-1)/2 */
    idx_t prior = (idx_t)(((__int128)r * (r - 1)) / 2);
    idx_t k = n - prior;    /* column index: 1 .. r */
    ensure_factorial(k);
    seq_t factk = fact[(size_t)k];
    seq_t val = factk * (seq_t)(r - k + 1);
    return val;
}
