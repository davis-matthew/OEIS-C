/*
 * A397186  Array read by ascending antidiagonals: A(n,k) = ((k + 1)^prime(n)
 *          - k^prime(n) - 1)/prime(n).
 * https://oeis.org/A397186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 38, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A397186 – array read by ascending antidiagonals.  
 * For each pair (k,n) with k,n≥1 define  
 *   A(k,n) = ((k+1)^prime(n) - k^prime(n) - 1) / prime(n)  
 * where prime(m) is the m‑th prime.  
 * The sequence a(n) lists the values of A(k,n) in order of increasing
 * sums k+n, i.e. first (1,1), then (1,2),(2,1), then (1,3),(2,2),(3,1),
 * etc.  
 */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_seq.h"

/* Return the n-th term of sequence A397186 (n ≥ 1).  */
seq_t a(idx_t n)
{
    /* Find the smallest s such that (s-1)*s/2 ≥ n.  */
    idx_t low = 2, high = 2;
    while (((unsigned __int128)high * (high - 1)) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = ((unsigned __int128)mid * (mid - 1)) / 2;
        if (val >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t s = low;                           /* current antidiagonal sum */

    /* Number of entries before this antidiagonal. */
    idx_t total_prev = ((unsigned __int128)(s - 2) * (s - 1)) / 2;

    /* Row (k) and column (n) indices in A(k,n).  */
    idx_t k = n - total_prev;                /* 1 ≤ k ≤ s-1 */
    idx_t nidx = s - k;                      /* 1 ≤ nidx ≤ s-1 */

    /* Compute the required prime and powers.  */
    uint32_t p = get_prime(nidx);            /* p = prime(nidx) */

    seq_t base_k = (seq_t)k;
    seq_t base_k1 = (seq_t)(k + 1);

    seq_t pow_k1 = pow_seq(base_k1, (int)p); /* (k+1)^p */
    seq_t pow_k  = pow_seq(base_k,  (int)p); /* k^p */

    seq_t num = pow_k1;
    num -= pow_k;
    num -= 1;                                /* numerator */

    return num / (seq_t)p;                   /* integer division */
}
