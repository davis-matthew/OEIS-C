/*
 * A328849  Numbers in whose primorial base expansion only even digits appear.
 * https://oeis.org/A328849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * Σ digit_k · primorial_k where digit_k is chosen by *
 *            expressing n–1 in the mixed radix system with radices *
 *            choices_k = (p_{k+1}+1)/2 * (p_k is the k‑th prime). The digits
 *            correspond to the even digits of * the primorial base expansion,
 *            so a(n) is twice the number obtained by * treati...
 * Range:     u8 n <= 24, u16 n <= 1203, u32 n <= 9072, u64 n <= 9072,
 *            u128 n <= 9072, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;                  /* a(1) = 0 */

    idx_t idx = n - (idx_t)1;             /* zero‑based mixed‑radix index */
    seq_t half = (seq_t)0;                /* H = Σ digit_k · primorial_k   */
    seq_t mult  = (seq_t)1;               /* primorial_k  (P_k)             */
    idx_t k = (idx_t)0;                   /* position counter */

    while (idx != (idx_t)0)
    {
        idx_t prime     = get_prime(k + (idx_t)1);          /* p_{k+1}   */
        seq_t choices_s = (seq_t)(prime + (idx_t)1) / (seq_t)2;
        idx_t choices   = (idx_t)choices_s;                /* (p_{k+1}+1)/2 */

        idx_t digit_index = idx % choices;
        idx /= choices;

        half += (seq_t)digit_index * mult;                 /* contribute */

        mult *= prime;                                    /* next primorial */
        ++k;
    }

    return half * (seq_t)2;                               /* a(n) = 2·half   */
}
