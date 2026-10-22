/*
 * A328625  In primorial base representation of n, multiply all other digits
 *          except the least significant with 1+{their right hand side
 *          neighbor}, and reduce each modulo prime(k) (with k > 1) to get a
 *          new digit for the position k (the least significant digit stays as
 *          it is), then convert back to decimal.
 * https://oeis.org/A328625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 9855, u32 n <= 9855, u64 n <= 9855,
 *            u128 n <= 9855, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A328625 – Primorial‐base digit transformation
   Formula: a(n) = A276085(A328624(n))         (see OEIS entry for details) */

#include <stdint.h>
#include <stddef.h>   /* for size_t */

/* First few primes – enough for the primorial base up to n≈10 000
   (the algorithm stops once the remaining value is zero).   */
static const uint32_t primes[30] = {
    2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U,
    23U, 29U, 31U, 37U, 41U, 43U, 47U, 53U,
    59U, 61U, 67U, 71U, 73U, 79U, 83U,
    89U, 97U, 101U, 103U, 107U, 109U, 113U
};

/* Convert n into primorial base, transform the digits, and convert back. */
seq_t a(idx_t n)
{
    /* --- 1. Convert n to primorial base digits (least significant first) --- */
    seq_t val = (seq_t)n;
    uint64_t digits[30];
    size_t len = 0;

    if (val == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        for (size_t i = 0; i < 30 && val != 0; ++i) {
            uint64_t div = primes[i];
            digits[i] = (uint64_t)(val % div);
            val = val / div;
            ++len;
        }
    }

    /* --- 2. Apply the A328624 transformation (and A276085 automatically) --- */
    uint64_t new_digits[30];
    new_digits[0] = digits[0];            /* least significant digit unchanged */
    for (size_t k = 1; k < len; ++k) {
        /* (digit[k] * (1 + digit[k-1])) mod prime[k]  (prime index = k) */
        uint64_t prod = digits[k] * (digits[k - 1] + 1ULL);
        new_digits[k] = prod % primes[k];
    }

    /* --- 3. Convert transformed digits back to decimal --- */
    seq_t result = 0;
    for (size_t i = len; i-- > 0; ) {     /* process from most significant to least */
        result = result * primes[i] + new_digits[i];
    }

    return result;
}
