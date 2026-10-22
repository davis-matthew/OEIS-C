/*
 * A325755  Numbers n divisible by their prime shadow A181819(n).
 * https://oeis.org/A325755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 2394, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n divisible by their prime shadow A181819(n) */

#include <stdint.h>
#include <stdlib.h>

/* Small list of the first 120 primes (index 1 => 2). */
static const uint32_t _prime_list_small[] = {
    2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
   31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
   73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
  127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
  179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
  233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
  283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
  353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
  419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
  467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
  547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
  607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
  661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
  739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
  811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
  877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
  947, 953, 967, 971, 977, 983, 991, 997,1009
};

static seq_t * _seq_cache   = NULL;
static size_t _seq_len     = 0;
static size_t _seq_cap     = 0;
static uint64_t _last_checked = 0;
static int _initialized = 0;

/* Ensure capacity for at least 'needed' elements. */
static void _ensure_seq_cache(size_t needed)
{
    if (_seq_cap < needed) {
        size_t new_cap = _seq_cap == 0 ? 16 : _seq_cap * 2;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(_seq_cache, new_cap * sizeof(seq_t));
        _seq_cache = tmp;
        _seq_cap   = new_cap;
    }
}

/* Compute the prime shadow of n: product of primes indexed by the exponents
   in the prime factorization of n. */
static seq_t _prime_shadow(uint64_t n)
{
    seq_t shadow = 1;
    if (n <= 1) return shadow;

    uint64_t temp = n;
    size_t prime_count = sizeof(_prime_list_small) / sizeof(_prime_list_small[0]);

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = _prime_list_small[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            /* exp >= 1, index is exp-1 */
            shadow *= (seq_t)_prime_list_small[exp - 1];
        }
    }
    if (temp > 1) {
        /* remaining prime factor gives exponent 1 */
        shadow *= (seq_t)_prime_list_small[0];  /* prime(1) = 2 */
    }
    return shadow;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (!_initialized) { _initialized = 1; }

    while (_seq_len < (size_t)n) {
        _last_checked++;
        seq_t sh = _prime_shadow(_last_checked);
        if (sh == 0) continue;          /* safety, should not happen */
        seq_t sn = (seq_t)_last_checked;
        if (sn % sh == 0) {
            _ensure_seq_cache(_seq_len + 1);
            _seq_cache[_seq_len++] = sn;
        }
    }
    return _seq_cache[(size_t)n - 1];
}
