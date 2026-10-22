/*
 * A387892  Psi-deficient numbers: numbers k such that psi(k) < 2*k.
 * https://oeis.org/A387892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates Psi‑deficient numbers.
   A number k is Psi‑deficient iff  
   ∏(1+1/p_i) < 2  for its distinct prime factors p_i.
   The function generates the sequence by testing successive integers
   and caching all valid terms. */

#include <stdint.h>
#include <stdlib.h>

/* ---- smallest prime factor sieve ----------------------------------- */

static uint64_t *spf = NULL;   /* spf[i] = smallest prime factor of i (i>=2) */
static size_t   spf_size = 0;  /* current size of spf array */

static void
init_spf(size_t limit)
{
    spf = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
    if (!spf) abort();
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint64_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = (uint64_t)i;
            }
        }
    }
    spf_size = limit;
}

static void
enlarge_spf(size_t new_limit)
{
    if (new_limit <= spf_size) return;
    size_t old = spf_size;
    spf = (uint64_t*)realloc(spf, (new_limit + 1) * sizeof(uint64_t));
    if (!spf) abort();
    for (size_t i = old + 1; i <= new_limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= new_limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint64_t)i;
            if (i * i <= new_limit) {
                for (size_t j = i * i; j <= new_limit; j += i)
                    if (!spf[j]) spf[j] = (uint64_t)i;
            }
        }
    }
    spf_size = new_limit;
}

/* ---- test whether a number satisfies the condition ---------------- */

static int
satisfies(uint64_t n)
{
    if (n == 1) return 1;            /* empty product = 1 < 2 */
    unsigned __int128 num = 1;       /* Π (p+1)   */
    unsigned __int128 den = 1;       /* Π p       */
    for (uint64_t x = n; x > 1; ) {
        uint64_t p = spf[x];
        num *= (unsigned __int128)(p + 1);
        den *= (unsigned __int128)p;
        while (x % p == 0) x /= p;
    }
    return num < (unsigned __int128)2 * den;
}

/* ---- cached enumeration of the sequence -------------------------- */

static seq_t  *cache      = NULL;
static size_t  cache_len  = 0;
static size_t  cache_cnt  = 0;
static uint64_t cur_val   = 1;          /* next integer to test */

/* ---- the required function --------------------------------------- */

seq_t a(idx_t n)
{
    /* one‑time initialization */
    static int initialized = 0;
    if (!initialized) {
        init_spf(1024);               /* start with a sieve of size 1024 */
        cache_len = 0;
        cache_cnt = 0;
        cur_val   = 1;
        initialized = 1;
    }

    /* ensure we find enough terms */
    while (cache_cnt < (size_t)n) {
        /* enlarge sieve if necessary */
        if (cur_val > spf_size)
            enlarge_spf(spf_size * 2 + 1);

        if (satisfies(cur_val)) {
            /* grow cache if needed */
            if (cache_cnt >= cache_len) {
                size_t new_len = cache_len ? cache_len * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_len = new_len;
            }
            cache[cache_cnt] = (seq_t)cur_val;
            ++cache_cnt;
        }
        ++cur_val;
    }

    return cache[n - 1];
}
