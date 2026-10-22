/*
 * A390725  Triangle read by rows: T(n, k) = Lah(n, k)*CatalanNumber(k), and
 *          Lah = A271703.
 * https://oeis.org/A390725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   locate the (n‑th) element in the triangle T(n,k) = Lah(n,k) *
 *            Catalan(k) where r = floor((sqrt(8n+1)-1)/2) (the row) k = n -
 *            r*(r+1)/2 (the column) For r==0,k==0 the value is 1. The
 *            function relies on the utilities binom(a,b) returning
 *            binomial(a,b) and catalan(k) returning the k‑th Catalan number
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 79, u64 n <= 193,
 *            u128 n <= 502, big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* compute the row r  (r = floor((sqrt(8*n+1)-1)/2)) */
    uint64_t r = 0;
    /* linear search is fine for n up to 10 000 (time limit 2 s) */
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }

    /* starting index of the row */
    uint64_t start = ((unsigned __int128)r * (r + 1)) / 2;
    uint64_t k = n - start;   /* column index */

    /* special cases for Lah numbers */
    if (r == 0 && k == 0) {
        return (seq_t)1;
    }
    if (k == 0 || k > r) {
        return (seq_t)0;
    }

    /* L(n,k) = binomial(r-1,k-1) * prod_{i=k+1}^{r} i */
    seq_t bin = binom((idx_t)(r - 1), (idx_t)(k - 1));

    seq_t prod = 1;
    for (uint64_t i = k + 1; i <= r; ++i) {
        prod *= (seq_t)i;
    }

    seq_t lah = bin * prod;
    seq_t cat = catalan(k);
    return lah * cat;
}
