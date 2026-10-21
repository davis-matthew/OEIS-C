/*
 * A293462  Let A_n be a square n X n matrix with entries A_n(i,j)=1 if i+j is
 *          a perfect power, and A_n(i,j)=0 otherwise. Then a(n) counts the
 *          1's in A_n.
 * https://oeis.org/A293462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p perfect power, 2≤p≤2n} (p-1 if p≤n+1 else 2n-p+1)
 * Range:     u8 n <= 50, u16 n <= 2043, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t limit = n * 2;
    /* Allocate array to mark perfect powers up to 2*n */
    bool *is_pp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_pp) return 0; /* allocation failure, treat as 0 */

    /* Generate all perfect powers ≤ limit, without duplicates */
    for (idx_t base = 2; base * base <= limit; ++base) {
        idx_t power = base * base;
        while (power <= limit) {
            is_pp[power] = true;
            if (power > limit / base) break; /* avoid overflow before next multiply */
            power *= base;
        }
    }

    seq_t count = 0;
    for (idx_t s = 2; s <= limit; ++s) {
        if (is_pp[s]) {
            if (s <= n + 1)
                count += (seq_t)(s - 1);
            else
                count += (seq_t)(2 * n - s + 1);
        }
    }

    free(is_pp);
    return count;
}
