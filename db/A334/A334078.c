/*
 * A334078  a(n) is the smallest positive integer that can be expressed as the
 *          difference of two positive squares in at least n ways.
 * https://oeis.org/A334078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the smallest positive integer that can be expressed as the
    difference of two positive squares in at least n ways.
    A representation a²‑b² corresponds to a factorisation
        d = (a‑b)(a+b)
    where the two factors have the same parity.
    For an odd d every divisor pair gives a representation;
    for d≡0 (mod 4), write d = 4k and the representations correspond
    to the divisor pairs of k.  Thus the number of representations of
    d is ⌊τ(d)/2⌋ for odd d, and ⌊τ(k)/2⌋ for d = 4k.
    Hence a(n) = min{ m | τ(m) ≥ 2n , m odd }   (odd m)
              or   4·k where τ(k) ≥ 2n           (m divisible by 4)
    The routine below finds, for a given t = 2n, the smallest integer
    with τ(·) ≥ t by a depth‑first search over prime exponent
    vectors.  Results are cached per t.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* List of the first 100 primes – enough for τ(·) up to 20 000 */
static const uint32_t PRIMES[100] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
    353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541
};
static const size_t PRIME_COUNT = 100;

/* 20000 is > 2*n for n≤10000 */
#define MAX_TARGET 20000
static seq_t _min_any[MAX_TARGET + 1] = {0};
static seq_t _min_odd[MAX_TARGET + 1] = {0};

/* Recursive search for the smallest number with τ≥target
   Starting from prime index `idx`, last exponent allowed `last_exp`,
   current value `val`, current divisor count `tau`.  */
static void dfs_min(size_t idx, unsigned last_exp,
                    seq_t val, unsigned tau,
                    unsigned target, seq_t *best)
{
    if (idx >= PRIME_COUNT) return;

    seq_t v = val;
    for (unsigned e = 1; e <= last_exp; ++e) {
        v = v * (seq_t)PRIMES[idx];

        /* prune if we already have a better candidate */
        if (*best && v >= *best) break;

        unsigned new_tau = tau * (e + 1);
        if (new_tau >= target) {
            if (*best == 0 || v < *best) *best = v;
        }
        dfs_min(idx + 1, e, v, new_tau, target, best);
    }
}

/* Computes minimal k with τ(k) ≥ target.
   `odd_only` == true forces the factorisation to exclude prime 2. */
static seq_t min_k_with_tau(unsigned target, bool odd_only)
{
    seq_t *cache = odd_only ? _min_odd : _min_any;
    if (cache[target]) return cache[target];

    seq_t best = 0;
    /* start index 0 for all primes, 1 for odd case only */
    size_t start = odd_only ? 1 : 0;
    dfs_min(start, 64, (seq_t)1, 1, target, &best);
    cache[target] = best;
    return best;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* outside offset, not used */
    unsigned target = (unsigned)((seq_t)n * 2);

    seq_t even_candidate = 0;
    seq_t odd_candidate  = 0;

    /* minimal k such that τ(k) >= target, any integer */
    seq_t k_any = min_k_with_tau(target, false);
    if (k_any) even_candidate = k_any * 4; /* m = 4*k */

    /* minimal odd k such that τ(k) >= target */
    seq_t k_odd = min_k_with_tau(target, true);
    if (k_odd) odd_candidate = k_odd;

    if (even_candidate && odd_candidate)
        return (even_candidate < odd_candidate) ? even_candidate : odd_candidate;
    if (even_candidate) return even_candidate;
    return odd_candidate; /* guaranteed to exist for target>=2 */
}
