/*
 * A278041  The tribonacci representation of a(n) is obtained by appending
 *          0,1,1 to the tribonacci representation of n (cf. A278038).
 * https://oeis.org/A278041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3 + Σ_{i=3}^{k} b_i * T_{i+3} where b_i are the bits of
 *            the Tribonacci representation of n, T_i is the Tribonacci
 *            sequence (T0=0,T1=0,T2=1,Tn=T_{n-1}+T_{n-2}+T_{n-3}), k = max{i
 *            | T_i <= n}. The bits are generated greedily
 * Range:     u8 n <= 40, u16 n <= 9609, u32 n <= 9609, u64 n <= 9609,
 *            u128 n <= 9609, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 */
    if (n == 0) return (seq_t)3;

    /* Tribonacci numbers up to the needed index  */
    seq_t T[200];
    T[0] = 0; T[1] = 0; T[2] = 1;
    idx_t i = 3;                       /* next free index */

    /* Grow T until T[i-1] > n (largest usable index will be k = i-2) */
    while ((idx_t)T[i-1] <= n) {
        T[i] = T[i-1] + T[i-2] + T[i-3];
        i++;
    }
    idx_t k = i - 2;                     /* largest index with T[k] <= n */

    /* Ensure we have T[k+3] for the contribution of each selected bit */
    while (i <= k + 3) {
        T[i] = T[i-1] + T[i-2] + T[i-3];
        i++;
    }

    seq_t sum = 3;                 /* contribution of the appended 011 */
    seq_t rem = n;

    /* Greedy extraction of bits and contribution to the sum */
    for (idx_t idx = k; idx >= 3; idx--) {
        if (rem >= T[idx]) {
            rem -= T[idx];
            sum += T[idx + 3];
        }
    }
    return sum;
}
