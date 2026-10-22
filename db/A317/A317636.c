/*
 * A317636  Minimum number of consecutive positive integers starting with 1
 *          that must be concatenated in descending order so that n divides
 *          the concatenation, or zero if n divides no such concatenation.
 * https://oeis.org/A317636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Search for the smallest k such that the decimal concatenation
 * of k, k-1, … , 1 is divisible by n.  Return 0 if no such k exists.
 * The algorithm keeps the current remainder of the concatenation
 * modulo n and the current power of 10 required to append the next
 * number.  It works for all n ≥ 1 and follows the OEIS definition
 * of sequence A317636.
 */
#include <stdbool.h>
#include <stdint.h>

static seq_t mul_mod_uint64(seq_t a, seq_t b, seq_t m) {
    /* a,b < m and m <= 10000 in practice, so 64‑bit intermediate is safe */
    return (seq_t)(( (unsigned long long)a * (unsigned long long)b ) % (unsigned long long)m);
}

seq_t a(idx_t n) {
    /* Even n or multiples of 5 never occur */
    if ((n % 2ULL == 0ULL) || (n % 5ULL == 0ULL)) return 0;
    if (n == 1ULL) return 1;   /* a(1) = 1 */

    const seq_t mod = (seq_t)n;          /* modulus n in seq_t */
    seq_t rem = 0;                       /* f(0) mod n */
    seq_t pow10_len = 1 % mod;           /* 10^len(f(0)) mod n */

    /* A generous upper bound; the search finishes much earlier for all known n */
    const idx_t MAX_K = 200000ULL;

    for (idx_t k = 1ULL; k <= MAX_K; ++k) {
        seq_t k_mod = (seq_t)k % mod;

        /* compute 10^len(k) mod mod */
        seq_t pow10_dk = 1 % mod;
        for (idx_t tmp = k; tmp > 0; tmp /= 10ULL) {
            pow10_dk = mul_mod_uint64(pow10_dk, (seq_t)10ULL, mod);
        }

        /* new remainder: k*10^len(prev) + prev */
        seq_t new_rem = rem + mul_mod_uint64(k_mod, pow10_len, mod);
        new_rem %= mod;
        if (new_rem == 0) return (seq_t)k;

        /* update for next iteration */
        rem = new_rem;
        pow10_len = mul_mod_uint64(pow10_len, pow10_dk, mod);
    }
    return 0;  /* Should not occur for admissible n */
}
