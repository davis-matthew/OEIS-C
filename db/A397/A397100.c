/*
 * A397100  a(n) is the number of steps by which n's trajectory under f is
 *          ahead of 0's; i.e., f^k(0) = f^(k-a(n))(n) for all sufficiently
 *          large k, where f(n) = n XOR 2^A000120(n).
 * https://oeis.org/A397100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy, look
 * Formula:   a(0)=0; a(2*n)=a(n)-2*popcount(n); a(2*n+1)=a(n)+1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t a_rec(idx_t n)
{
    if (n == 0) return 0;
    if ((n & 1ull) == 0) {                /* n even */
        idx_t m  = n >> 1;
        seq_t h  = a_rec(m);
        seq_t pc = (seq_t)__builtin_popcountll(m);
        return h - ((seq_t)2 * pc);
    } else {                              /* n odd */
        return a_rec(n >> 1) + 1;
    }
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
