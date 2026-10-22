/*
 * A359507  a(n) is the least integer k such that there exists a strictly
 *          increasing integer sequence n = b_1 < b_2 < ... < b_t = n + k with
 *          the property that b_1 XOR b_2 XOR ... XOR b_t = 0.
 * https://oeis.org/A359507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer k such that there exists a strictly
 *            increasing integer sequence n = b1 < b2 < ... < bt = n + k with
 *            the property b1 XOR b2 XOR ... XOR bt = 0. This implementation
 *            computes the minimal k by iteratively constructing the
 *            linear‑basis of the intermediate integers {n+1,…,n+k-1} an...
 * Range:     u8 n <= 382, u16 n <= 7131, u32 n <= 7131, u64 n <= 7131,
 *            u128 n <= 7131, big any n
 * Helpers:   in_span, insert_basis
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/in_span.h"
#include "../utils/insert_basis.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned long long nn = n;
    unsigned long long basis[64] = {0};
    unsigned long long L;             /* length of the intermediate block */
    unsigned long long min_k = 0;     /* result to return */

    for (L = 1; ; ++L) {
        unsigned long long new_num = nn + L;
        insert_basis(basis, new_num);

        unsigned long long target = nn ^ (nn + L + 1);   /* n XOR (n+L+1) */
        if (in_span(basis, target)) {
            min_k = L + 1;   /* k = L + 1 */
            break;
        }
    }
    return (seq_t)min_k;
}
