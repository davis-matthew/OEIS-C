/*
 * A353813  a(n) = 1 if n has exactly one prime factor of form 4*k+1 (when
 *          counted with multiplicity) and no prime factor 4*k+3 with odd
 *          multiplicity, otherwise 0.
 * https://oeis.org/A353813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has exactly one prime factor ≡ 1 (mod 4) counting
 *            multiplicity and all prime factors ≡ 3 (mod 4) occur with even
 *            multiplicity; otherwise 0
 * Range:     u8 n <= 9758, u16 n <= 9758, u32 n <= 9758, u64 n <= 9758,
 *            u128 n <= 9758, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static const size_t MAX_FACTORS = 64;

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    uint64_t primes_arr[MAX_FACTORS];
    uint64_t exps_arr[MAX_FACTORS];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes_arr, exps_arr, &cnt);

    unsigned one_mod4_count = 0;
    bool bad = false;

    for (size_t i = 0; i < cnt && !bad; ++i) {
        uint64_t p = primes_arr[i];
        uint64_t e = exps_arr[i];

        if (p == 2)
            continue;

        if ((p & 3) == 1) {          /* p ≡ 1 (mod 4) */
            if (e != 1 || one_mod4_count == 1)
                bad = true;
            else
                one_mod4_count = 1;
        }
        else if ((p & 3) == 3) {      /* p ≡ 3 (mod 4) */
            if (e & 1)
                bad = true;
        }
    }

    return (bad || one_mod4_count != 1) ? (seq_t)0 : (seq_t)1;
}
