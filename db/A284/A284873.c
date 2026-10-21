/*
 * A284873  Array read by antidiagonals: T(n,k) = number of double palindromes
 *          of length n using a maximum of k different symbols.
 * https://oeis.org/A284873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 98, u32 n <= 315, u64 n <= 932,
 *            u128 n <= 1275, big any n
 * Helpers:   big_pow, phi_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A284873:  Array read by antidiagonals
   T(n,k) = r(n,k) – Σ_{d|n,d<n} φ(n/d) · T(d,k)
   r(2d,k)   = d·(k+1)·k^d
   r(2d+1,k) = (2d+1)·k^(d+1)
   The sequence a(n) is obtained by reading the array T(n,k)
   along antidiagonals: (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),...

   The implementation uses memoisation and a simple divisor loop.
   Small integers (≤151) are handled with normal arithmetic; 
   larger intermediary results use the arbitrary‑precision type seq_t.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   // big_pow(seq_t base, seq_t exp)
#include "../utils/phi_int.h"

/* maximum n and k that can occur for a(n) with n up to 10 000
   (satisfies s(s-1)/2 ≥ 10000 → s ≈ 142, hence n,k ≤ 141) */
#define MAX_N 150

static seq_t T_table[MAX_N + 1][MAX_N + 1];
static bool   T_computed[MAX_N + 1][MAX_N + 1];

/* recursive, memoised computation of T(n,k) */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (T_computed[n][k]) return T_table[n][k];

    seq_t val;
    if (n == 1) {
        val = (seq_t)k;                       /* r(1,k) = k, no sum */
    } else {
        seq_t r;
        if ((n & 1) == 0) {                  /* even n = 2d */
            idx_t d = n >> 1;
            seq_t powv = big_pow((seq_t)k, (seq_t)d);
            r = (seq_t)d * ((seq_t)(k + 1) * powv);
        } else {                              /* odd n = 2d+1 */
            idx_t d = (n - 1) / 2;
            seq_t powv = big_pow((seq_t)k, (seq_t)(d + 1));
            r = (seq_t)n * powv;
        }

        seq_t sum = 0;
        for (idx_t d = 1; d < n; ++d) {
            if (n % d == 0) {
                idx_t nd = n / d;
                seq_t phi = (seq_t)phi_int((int)nd);
                seq_t sub = compute_T(d, k);
                sum += phi * sub;
            }
        }
        val = r - sum;
    }

    T_table[n][k] = val;
    T_computed[n][k] = true;
    return val;
}

/* ------------------------------------------------------------
   Main entry: a(n) – value of A284873 at index n (1‑based).
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* find the smallest s with s*(s-1)/2 ≥ n   (s ≥ 2) */
    idx_t s = 1;
    while ((s * (s - 1)) / 2 < n) ++s;

    /* cumulative count before block s : (s-2)(s-1)/2 */
    idx_t prevC = ((s - 1) * (s - 2)) / 2;
    idx_t t = n - prevC - 1;                    /* 0‑based offset in block */
    idx_t i = t + 1;                            /* row index n */
    idx_t k = (s - 1) - t;                      /* column k */

    return compute_T(i, k);
}
