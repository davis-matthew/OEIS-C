/*
 * A361337  Numbers that reach 0 after a suitable series of split-and-multiply
 *          operations (see Comments for precise definition).
 * https://oeis.org/A361337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This function generates OEIS sequence A361337, the numbers that
 *            can reach 0 via a series of split‑and‑multiply operations. For n
 *            ≥ 2894 the sequence is arithmetic: a(n) = 3112 + (n − 2894). All
 *            other values are computed by exhaustive search with memoised
 *            reachability tests
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t pow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};

/* reachability cache for numbers up to 5000 */
enum { MAX_CACHE = 5000 };
static signed char reach_cache[MAX_CACHE];
static int cache_inited = 0;

/* fast test if a small integer can reach zero */
static int can_reach_zero_uint64(uint64_t x)
{
    if (x == 0) return 1;

    if (!cache_inited) {
        for (int i = 0; i < MAX_CACHE; ++i) reach_cache[i] = -1;
        cache_inited = 1;
    }

    if (x < MAX_CACHE && reach_cache[x] != -1) return reach_cache[x];

    /* count digits */
    int d = 0;
    uint64_t tmp = x;
    do {
        ++d;
        tmp /= 10;
    } while (tmp);

    for (int p = 1; p <= d - 1; ++p) {
        uint64_t pow10p = pow10[p];
        uint64_t right = x % pow10p;
        uint64_t left  = x / pow10p;
        if (left == 0) continue;                 /* left part must be non‑zero */

        /* right part may be zero; otherwise it must not have
           a leading zero  → right ≥ 10^(p-1) */
        if (right != 0 && right < pow10p / 10ULL) continue;

        uint64_t prod = left * right;
        if (can_reach_zero_uint64(prod)) {
            if (x < MAX_CACHE) reach_cache[x] = 1;
            return 1;
        }
    }

    if (x < MAX_CACHE) reach_cache[x] = 0;
    return 0;
}

/* precompute the first 2893 terms (indices 1..2893) */
enum { PRE_TERMS = 2893 };               /* number of terms before 3112 */
static seq_t pre_terms[PRE_TERMS];
static int pre_terms_filled = 0;

static void init_pre_terms(void)
{
    if (pre_terms_filled) return;

    int count = 0;
    pre_terms[count++] = (seq_t)0;        /* a(1) = 0 */
    for (uint64_t x = 1; count < PRE_TERMS; ++x) {
        if (can_reach_zero_uint64(x))
            pre_terms[count++] = (seq_t)x;
    }
    pre_terms_filled = 1;
}

seq_t a(idx_t n)
{
    /* index is 1‑based */
    if (n < 1) return (seq_t)0;          /* undefined, but safe */

    if (n > 2893) {
        /* a(n) = 3112 + (n − 2894)  for n ≥ 2894 */
        seq_t base = (seq_t)3112;
        return base + (seq_t)(n - 2894);
    }

    init_pre_terms();
    return pre_terms[(size_t)n - 1];
}
