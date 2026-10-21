/*
 * A280049  Canonical representation of n as a sum of distinct Jacobsthal
 *          numbers J(n) (A001045) (see Comments for details); also binary
 *          numbers that end in an even number of zeros.
 * https://oeis.org/A280049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(A003159(n)) – canonical representation of n * as
 *            a sum of distinct Jacobsthal numbers with the smallest summand *
 *            having an odd index; the bits of the representation are then
 *            read * as a decimal integer composed of 0’s and 1’s
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 682, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_J_IDX 200   /* sufficient for n up to 2^64 */

seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* not used, defensive */

    idx_t N = (idx_t)n;              /* operates purely on idx_t */
    idx_t J[MAX_J_IDX + 1];
    bool used[MAX_J_IDX + 1];
    memset(used, 0, sizeof(used));

    /* pre‑compute Jacobsthal numbers J(k) = J(k‑1) + 2*J(k‑2) */
    J[0] = 0;
    J[1] = 1;
    J[2] = 1;
    int max_k = 2;

    int i;
    for (i = 3; i <= MAX_J_IDX; ++i) {
        /* guard against overflow: if J[i-1] > N or
         * J[i-2] > N/2 then J[i] would be > N, no need to compute it */
        if (J[i-1] > N || J[i-2] > N / 2) {
            max_k = i - 1;
            break;
        }
        J[i] = J[i-1] + (J[i-2] << 1);
        if (J[i] > N) {
            max_k = i - 1;
            break;
        }
        max_k = i;
    }
    if (i > MAX_J_IDX) max_k = i - 1;   /* reached array limit */

    /* greedy selection of the largest possible Jacobsthal numbers */
    idx_t rem = N;
    for (i = max_k; i >= 1; --i) {
        if (rem >= J[i]) {
            used[i] = true;
            rem -= J[i];
        }
    }

    /* the greedy algorithm may leave the smallest used index even.
     * Replace J(m) with all J(1..m-1) when m is even (property:
     * J(m) = Σ_{k=1}^{m-1} J(k) for even m). */
    int smallest = 0;
    for (i = 1; i <= max_k; ++i) {
        if (used[i]) { smallest = i; break; }
    }
    if (smallest && ((smallest & 1) == 0)) {
        used[smallest] = false;
        for (i = 1; i < smallest; ++i) used[i] = true;
    }

    /* convert the bit pattern to a decimal number composed of 0’s and 1’s */
    seq_t result = 0;
    for (i = max_k; i >= 1; --i) {
        result = result * 10 + (used[i] ? 1 : 0);
    }
    return result;
}
