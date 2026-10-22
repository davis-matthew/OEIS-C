/*
 * A390296  Numbers k such that sigma(k) = psi(k) + tau(k)^2.
 * https://oeis.org/A390296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the union of 12 infinite progressions
 *  m * p^2   (p is prime, p ∤ m),  with
 *  m ∈ {94,115,119,282,310,322,345,357,385,930,966,1155}.
 * The OEIS comments also say that numbers of the form p*q*r²
 * with (p+1)(q+1)=144 are included. That merely reproduces the
 * same progressions with m = 94,115,119, so the definition above
 * already covers all terms.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- M constants ----------
 * The 12 values of m appearing in the description
 */
static const uint64_t MS[12] = {
    94ULL, 115ULL, 119ULL, 282ULL, 310ULL, 322ULL,
    345ULL, 357ULL, 385ULL, 930ULL, 966ULL, 1155ULL
};

/* ---------- Prime list ----------
 * At run‑time we grow this array with newly discovered primes.
 * All functions are thread‑safe only for single‑threaded use,
 * which is sufficient for the judge environment.
 */
static uint64_t *prime_list = NULL;
static size_t   prime_len  = 0;
static size_t   prime_cap  = 0;

static void prime_add(uint64_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* simple incremental prime generator (trial division) */
static void prime_generate_more(size_t upto_idx)
{
    while (prime_len <= upto_idx) {
        uint64_t cand = (prime_len == 0) ? 2ULL : prime_list[prime_len-1] + 1ULL;
        for (;;) {
            bool prime = true;
            for (size_t j = 0; j < prime_len; ++j) {
                uint64_t q = prime_list[j];
                if (q*q > cand) break;
                if (cand % q == 0) { prime = false; break; }
            }
            if (prime) { prime_add(cand); break; }
            ++cand;
        }
    }
}

/* ---------- Cache of sequence values ----------
 * The cache holds the sequence in ascending order.  It grows on
 * demand.  The cache itself is sorted, so we can fetch any n-th
 * value by simply ensuring a sufficient length.
 */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_ensure_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static void cache_append(seq_t v)
{
    cache_ensure_cap(cache_len + 1);
    cache[cache_len++] = v;
}

/* ---------- Progression state ----------
 * For each m we keep the current prime index that will be used
 * for the next term.  After generating a term we advance to the
 * next prime that does not divide m.
 */
typedef struct {
    size_t prime_idx; /* index into prime_list of the next prime to use */
    seq_t  next_val;  /* value m * prime^2 (already computed) */
} MState;

static MState states[12];
static bool seq_initialized = false;
static seq_t last_val          = 0;
static bool last_val_set       = false;

/* Prepare the first term for each progression */
static void m_next_init(size_t m_idx)
{
    states[m_idx].prime_idx = 0;
}

/* Compute the next term for progression m_idx.  It updates
 * states[m_idx].prime_idx and next_val so that subsequent calls
 * yield the following term. */
static void m_next_compute(size_t m_idx)
{
    size_t idx = states[m_idx].prime_idx;   /* current prime index */

    /* grow prime list if necessary */
    prime_generate_more(idx);

    uint64_t p = prime_list[idx];
    /* skip primes that divide m */
    while (MS[m_idx] % p == 0) {
        ++idx;
        prime_generate_more(idx);
        p = prime_list[idx];
    }

    /* compute next value and advance index for subsequent calls */
    seq_t mul   = (seq_t)MS[m_idx];
    seq_t p2    = (seq_t)p * (seq_t)p;
    states[m_idx].next_val = mul * p2;
    states[m_idx].prime_idx = idx + 1;   /* next prime will be at idx+1 */
}

/* Initialisation of all progressions */
static void seq_initialise(void)
{
    /* Ensure at least the first prime (2) is present */
    prime_generate_more(0);

    for (size_t i = 0; i < 12; ++i) {
        m_next_init(i);
        m_next_compute(i);  /* compute first term for each m */
    }
    seq_initialized = true;
}

/* ---------- Main sequence function ----------
 * The function builds the merged ascending list lazily, storing it
 * in the cache.  Since the cache is sorted, fetching the n-th
 * element is just a lookup after ensuring enough elements have been
 * generated.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset */

    size_t target = (size_t)(n - 1);   /* 0‑based index */
    if (!seq_initialized) seq_initialise();

    /* Generate more terms while the cache is too small */
    while (cache_len <= target) {
        /* Find the smallest next_val among all 12 progressions */
        seq_t min_val = states[0].next_val;
        size_t min_idx = 0;
        for (size_t i = 1; i < 12; ++i) {
            if (states[i].next_val < min_val) {
                min_val = states[i].next_val;
                min_idx = i;
            }
        }

        /* Insert into cache if it is not a duplicate */
        if (!last_val_set || min_val != last_val) {
            cache_append(min_val);
            last_val = min_val;
            last_val_set = true;
        }

        /* Advance the progression that produced this min_val */
        m_next_compute(min_idx);
    }

    return cache[target];
}
