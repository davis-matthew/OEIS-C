/*
 * A269158  Square array A(row,col) = F(row,(2*col)-1), where F(0,q) = F(1,q)
 *          = 0, F(2p,q) = F(p,q) XOR A003188(q), F(2p+1,q) = F(q mod 2p+1,
 *          2p+1) XOR (2p+1 AND q). Array is read by descending antidiagonals
 *          as A(1,1), A(1,2), A(2,1), A(1,3), A(2,2), A(3,1), ...
 * https://oeis.org/A269158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8386, u16 n <= 9865, u32 n <= 9865, u64 n <= 9865,
 *            u128 n <= 9865, big any n
 * Helpers:   diagonal_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of OEIS sequence A269158.
    The array is defined by A(row,col)=F(row,2*col-1)
    with F(p,q) (p >1, q odd) defined by:
        F(p,q) = 0                     if p <= 1
        F(p,q) = F(q mod p, p) XOR (p & q)     if p is odd
        F(p,q) = F(p/2,q)  XOR Gray(q)          if p is even
    where Gray(q)=q XOR (q>>1) (the binary reflected Gray code of q).
    The sequence is obtained by reading the array along descending
    antidiagonals: (1,1), (1,2), (2,1), (1,3), (2,2), (3,1), …  */

/* Standard inclusions – required for integer types and
   unsigned __int128 used in triangular computations. */
#include <stdint.h>
#include "../utils/diagonal_index.h"

/* Forward declaration of the recursive definition of F. */
static seq_t f_recursive(seq_t p, seq_t q);

/* Helper: the Gray‑code of an odd number q (q>>1 is defined for odd q). */
static inline seq_t gray_code(seq_t q) {
    return q ^ (q >> 1);
}

/* Recursive evaluation of F as described above. */
static seq_t f_recursive(seq_t p, seq_t q)
{
    if (p <= 1) {
        return (seq_t)0;
    }
    if (p & (seq_t)1) {            /* p is odd */
        seq_t mod = q % p;
        return f_recursive(mod, p) ^ (p & q);
    } else {                       /* p is even */
        return f_recursive(p >> 1, q) ^ gray_code(q);
    }
}

/* Main sequence function: a(n) for n>=1. */
seq_t a(idx_t n)
{
    /* 1‑based index */
    uint64_t N = (uint64_t)n;                /* explicit cast for clarity */
    uint64_t S = diagonal_index(N);          /* diagonal number (sum r+c-1) */
    uint64_t prev_tri = (uint64_t)(((__uint128_t)S * (S - 1)) / 2);
    uint64_t pos = N - prev_tri - 1;         /* zero‑based position inside diagonal */
    uint64_t row = pos + 1;
    uint64_t col = S - row + 1;

    seq_t p = (seq_t)row;                    /* row as seq_t */
    seq_t q = (seq_t)((seq_t)2 * col - 1);   /* column mapped to odd q */
    return f_recursive(p, q);
}
