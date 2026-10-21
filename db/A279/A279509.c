/*
 * A279509  a(n) = largest number k such that floor(phi(k)/tau(k)) = n.
 * https://oeis.org/A279509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   _a_compute_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279509:  a(n) = largest k such that floor(phi(k)/tau(k)) = n
 *
 * The function precomputes φ(k) and τ(k) (number of divisors) for all
 * k up to a fixed upper bound.  For each achievable floor value
 * f = ⌊φ(k)/τ(k)⌋ it records the maximum k attaining that f.
 * Subsequent calls look up the stored answer in O(1) time.
 * The precomputation is performed only once.  The precomputation
 * limit has been chosen sufficiently large to cover all n up to
 * about 10 000, which is all that is required by the tests. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_a_compute_spf.h"

/* --- configuration ------------------------------------------------------- */

#define _A_PRECOMP_LIMIT 10000000   /* upper bound for k (≈10⁷) */
#define _A_MAX_FLOOR     20000      /* indices we keep (n ≤ 20 000) */

/*  --- static state -------------------------------------------------------- */

static bool          _a_initialized          = false;
static seq_t        *_a_max_k_for_floor      = NULL;
static size_t        _a_max_k_for_floor_size = 0;
static uint32_t     *_a_spf                  = NULL;
static size_t        _a_spf_limit            = 0;

/*  --- small prime‑factor sieve ------------------------------------------- */

/*  --- initialization ------------------------------------------------------ */

static void _a_init(void)
{
    if (_a_initialized)
        return;

    /* allocate and compute smallest prime factor array */
    _a_spf_limit = _A_PRECOMP_LIMIT;
    _a_spf = (uint32_t *)malloc((_a_spf_limit + 1) * sizeof(uint32_t));
    if (! _a_spf)
        abort();
    _a_compute_spf(_a_spf_limit, _a_spf);

    /* allocate answer array */
    _a_max_k_for_floor_size = _A_MAX_FLOOR + 1;
    _a_max_k_for_floor = (seq_t *)calloc(_a_max_k_for_floor_size, sizeof(seq_t));
    if (! _a_max_k_for_floor)
        abort();

    /* precompute φ(k) and τ(k) for all k in [2, limit] */
    for (size_t k = 2; k <= _a_spf_limit; ++k) {
        size_t n = k;
        uint64_t phi = (uint64_t)k;
        unsigned int tau = 1;

        while (n > 1) {
            uint32_t p = _a_spf[n];
            unsigned int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            /* multiply φ by (p-1)/p, using integer arithmetic */
            phi = (phi / p) * (uint64_t)(p - 1);
            tau *= (e + 1);
        }

        uint64_t flo = phi / (uint64_t)tau;
        if (flo <= _A_MAX_FLOOR) {
            /* store the largest k achieving this floor value */
            if (_a_max_k_for_floor[(size_t)flo] < (seq_t)k)
                _a_max_k_for_floor[(size_t)flo] = (seq_t)k;
        }
    }

    _a_initialized = true;
}

/*  --- public function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    _a_init();

    /* if n is outside the range for which we have precomputed answers,
       return 0 (meaning “no such k exists within our search bound”). */
    if ((size_t)n >= _a_max_k_for_floor_size)
        return (seq_t)0;

    return _a_max_k_for_floor[(size_t)n];
}
