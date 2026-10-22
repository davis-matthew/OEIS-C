/*
 * A308376  Least positive integer k < prime(n) such that k + k' is a square,
 *          where k' is the unique integer with 0 < k < prime(n) and k*k' == 1
 *          (mod prime(n)), or 0 if such a number k does not exist.
 * https://oeis.org/A308376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = least k < prime(n) such that k + k' is a perfect square,
 * where k' is the unique integer with 0 < k' < prime(n) and k * k' ≡ 1 (mod prime(n)),
 * or 0 if no such k exists.
 * The computation is performed by brute‑force search over k = 1 … prime(n)-1.
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety, though offset ensures n≥1 */

    seq_t p = get_prime(n);      /* prime(n) */
    seq_t best = 0;

    for (seq_t k = 1; k < p; ++k) {
        /* modular inverse via Fermat: k^(p-2) mod p */
        seq_t kinv = mod_pow(k, (uint64_t)(p - 2), p);

        seq_t sum = k + kinv;
        seq_t r   = floor_sqrt(sum);

        if (r * r == sum) {
            best = k;
            break;              /* smallest k found */
        }
    }
    return best;
}
