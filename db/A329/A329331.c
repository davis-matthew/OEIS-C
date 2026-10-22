/*
 * A329331  Binary operation over the nonnegative integers, distributive over
 *          A003987(.,.), such that A(2^i, 2^j) = 2^A054237(i,j). Square array
 *          A(n,k), n >= 0, k >= 0, read by descending antidiagonals.
 * https://oeis.org/A329331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   bitpos_to_pair, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A329331 – Binary operation over the nonnegative integers
    Compute a(n) as the GF(2)[x,y] product of the binary
    representations of the row and column indices obtained
    from the linear index n by reading the matrix
    by descending antidiagonals.  */
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/bitpos_to_pair.h"

/*  A(a,b) : the binary operation described in the OEIS entry.
    It is the product in the ring GF(2)[x,y] of the two
    polynomials corresponding to the binary expansions of a
    and b, with the mapping of powers of two described above. */
static seq_t compute_A(idx_t a, idx_t b)
{
    seq_t result = 0;
    /* iterate over bits set in a */
    idx_t bits_a = a;
    while (bits_a) {
        idx_t pos_a = __builtin_ctzll(bits_a);
        idx_t i_a, j_a;
        bitpos_to_pair(pos_a, &i_a, &j_a);
        bits_a &= bits_a - 1;

        /* iterate over bits set in b */
        idx_t bits_b = b;
        while (bits_b) {
            idx_t pos_b = __builtin_ctzll(bits_b);
            idx_t i_b, j_b;
            bitpos_to_pair(pos_b, &i_b, &j_b);
            bits_b &= bits_b - 1;

            /* sum of exponents */
            idx_t i_res = i_a + i_b;
            idx_t j_res = j_a + j_b;
            idx_t s_res = i_res + j_res;
            idx_t t_res = s_res * (s_res + 1) / 2;
            idx_t pos_res = t_res + j_res;

            /* toggle the corresponding bit in the result */
            result ^= pow2(pos_res);
        }
    }
    return result;
}

/*  Main routine requested by the framework.
    Convert the linear index n into the pair (row,col)
    according to a descending‑antidiagonal enumeration,
    then compute the corresponding entry of the array. */
seq_t a(idx_t n)
{
    /* Find the diagonal sum s and the triangular offset T_s. */
    idx_t s = 0;
    idx_t t = 0;          /* T_s */
    while (t + s + 1 <= n) {
        ++s;
        t = s * (s + 1) / 2;
    }
    idx_t col = n - t;    /* j */
    idx_t row = s - col;  /* i */

    return compute_A(row, col);
}
