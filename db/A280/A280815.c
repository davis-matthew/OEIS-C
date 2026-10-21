/*
 * A280815  a(n) = A049502(cototient(n)).
 * https://oeis.org/A280815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049502(A051953(n))
 * Range:     u8 n <= 1274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   major_index_binary, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = major index (2nd definition) of cototient(n) */

/* external function required by the framework */
#include "../utils/phi.h"
#include "../utils/major_index_binary.h"

seq_t a(idx_t n)
{
    /* cototient(n) = n - phi(n) */
    seq_t phi_n = phi(n);           /* Euler's totient of n */
    seq_t cot   = (seq_t)n - phi_n; /* n - phi(n) */

    /* major index (2nd definition) of cototient(n) */
    return major_index_binary(cot);
}
