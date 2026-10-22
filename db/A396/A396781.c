/*
 * A396781  a(n) is the largest integer of the form r^k (with 0 < r < n and k
 *          > 0) whose base-n digital root is r, and whose number of digits in
 *          base-n is k.
 * https://oeis.org/A396781
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n-1)^k, where k is the largest integer satisfying
 *            n^{k-1} ≤ (n-1)^k. Compute k via k = floor( ln n / ( ln n –
 *            ln(n‑1) ) ), then adjust for rounding errors. See OEIS A396781
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined below offset, safe default */

    long double ln_n  = logl((long double)n);
    long double ln_n1 = logl((long double)(n - 1));
    long double k_est = floorl(ln_n / (ln_n - ln_n1) + 1e-15L);
    idx_t k = (idx_t)k_est;
    if (k < 1)
        k = 1;

    seq_t n_seq  = (seq_t)n;
    seq_t n1_seq = (seq_t)(n - 1);

    /* Adjust k if the floating‑point estimate was off by ±1 */
    while (1) {
        seq_t pow_n_k   = pow_seq(n_seq,  (int)k);
        seq_t pow_n1_k  = pow_seq(n1_seq, (int)k);
        seq_t pow_n1_kp1 = pow_seq(n1_seq, (int)(k + 1));

        if (pow_n_k <= pow_n1_kp1) {       /* can increase k */
            k++;
            continue;
        }

        if (k > 1) {
            seq_t pow_n_km1 = pow_seq(n_seq, (int)(k - 1));
            if (pow_n_km1 > pow_n1_k) {  /* must decrease k */
                k--;
                continue;
            }
        }
        break;   /* k is correct */
    }

    return pow_seq(n1_seq, (int)k);
}
