/*
 * A380110  In the base 4 expansion of n: map 0->0, 1->1, 2->1, 3->2.
 * https://oeis.org/A380110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum over base‑4 digits d of f(d)·4^k, where f(0)=0,
 *            f(1)=1, f(2)=1, f(3)=2. Equivalently, a(n) = n – Σ_{k:
 *            digit_k≥2} 4^k
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = (seq_t)n;          /* start with the original value   */
    idx_t  x   = n;                /* iterate over base‑4 digits       */
    idx_t  pos = 0;                /* power index k                     */
    while (x) {
        uint8_t d = x & 3;         /* x % 4  (fast)                     */
        if (d >= 2) {              /* digit 2 or 3 needs subtraction    */
            res -= ((seq_t)1) << (2 * pos);  /* 4^pos = 1 << (2*pos)          */
        }
        x >>= 2;                   /* divide x by 4 (shift right 2)     */
        ++pos;
    }
    return res;
}
