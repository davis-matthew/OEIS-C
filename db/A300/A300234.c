/*
 * A300234  a(n) = number of steps in simple Euclidean algorithm for gcd(n,k)
 *          to reach the termination test n=k when starting with n = n and k =
 *          phi(n).
 * https://oeis.org/A300234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subtraction steps of Euclid's algorithm for
 *            gcd(n,phi(n)), i.e. a(n) = A285721(n,phi(n))
 * Range:     u8 n <= 255, u16 n <= 9748, u32 n <= 9748, u64 n <= 9748,
 *            u128 n <= 9748, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

/* compute number of subtraction steps until two numbers are equal */
static seq_t sub_steps(seq_t a, seq_t b)
{
    seq_t steps = 0;
    while (a != b) {
        if (a > b) {
            seq_t q = a / b;
            seq_t r = a % b;
            if (r == 0) {
                steps += q - 1;
                break;          /* a will become equal to b */
            } else {
                steps += q;
                a = r;
            }
        } else {                  /* b > a */
            seq_t q = b / a;
            seq_t r = b % a;
            if (r == 0) {
                steps += q - 1;
                break;          /* b will become equal to a */
            } else {
                steps += q;
                b = r;
            }
        }
    }
    return steps;
}

/* OEIS sequence A300234: a(n) = number of subtraction steps to reach
   equality for (n,phi(n)). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but handle 0 safely */
    seq_t phi_n = phi(n);
    return sub_steps((seq_t)n, phi_n);
}
