/*
 * A319069  a(1) = a(2) = 1; a(n) = a(n-A000005(n-1)) + a(n-A000005(n-2)) for
 *          n > 2.
 * https://oeis.org/A319069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-τ(n-1)) + a(n-τ(n-2)), a(1)=a(2)=1 * where τ(k) is
 *            the number of positive divisors of k
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

static seq_t *A = NULL;          /* cache of a(n) values */
static size_t A_cap = 0;         /* allocated capacity of A */
static idx_t  A_max = 0;         /* largest index actually stored */

static uint16_t *tau = NULL;     /* cache of divisor counts τ(k) */
static size_t tau_cap = 0;       /* allocated capacity of tau */

static void ensure_cache(idx_t n)
{
    if (n + 1 <= A_cap) /* already enough space */
        return;

    size_t new_cap = (size_t)(n + 1);

    seq_t *newA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!newA) exit(EXIT_FAILURE);
    A = newA;
    A_cap = new_cap;

    uint16_t *newTau = (uint16_t *)realloc(tau, new_cap * sizeof(uint16_t));
    if (!newTau) exit(EXIT_FAILURE);
    tau = newTau;
    tau_cap = new_cap;

    for (idx_t i = A_max + 1; i <= n; ++i) {
        tau[i] = (uint16_t)divisor_count_u64((uint64_t)i);
        if (i <= 2u) {
            A[i] = (seq_t)1;
        } else {
            idx_t idx1 = i - tau[i - 1];
            idx_t idx2 = i - tau[i - 2];
            A[i] = A[idx1] + A[idx2];
        }
    }

    A_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out-of-bounds, not used by OEIS offset */
    ensure_cache(n);
    return A[n];
}
