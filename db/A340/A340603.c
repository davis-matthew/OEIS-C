/*
 * A340603  Heinz numbers of integer partitions of odd rank.
 * https://oeis.org/A340603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th Heinz number of a partition whose Dyson rank is
 *            odd This implementation searches the integers sequentially,
 *            testing the parity of the rank of each Heinz number using a
 *            dynamic sieve for prime factorizations. It allocates the sieve
 *            tables with calloc/malloc and casts the void* ret...
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- dynamic sieve data ---------------------------------------- */
static idx_t *spf = NULL;          /* smallest prime factor of each integer */
static idx_t *prime_index = NULL;  /* 1‑based index of each prime */
static idx_t *primes = NULL;       /* list of found primes */
static idx_t cur_limit = 0;        /* current upper bound of the sieve */
static size_t primes_cnt = 0;      /* number of primes stored */

/* ---------- build or rebuild the sieve up to 'limit' ------------------- */
static void build_sieve(idx_t limit)
{
    /* free old tables (if any) */
    free(spf);
    free(prime_index);
    free(primes);

    /* allocate new tables */
    spf          = (idx_t*)calloc((size_t)(limit + 1), sizeof(idx_t));
    prime_index  = (idx_t*)calloc((size_t)(limit + 1), sizeof(idx_t));
    primes       = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf || !prime_index || !primes) {
        /* in a production build one would handle the error; here we
           simply abort to keep the code small and portable */
        abort();
    }

    size_t pc = 0;   /* current count of primes */

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            primes[pc] = i;
            prime_index[i] = (idx_t)(pc + 1); /* 1‑based */
            ++pc;
        }
        /* linear sieve step */
        for (size_t j = 0; j < pc &&
                primes[j] <= spf[i] &&
                (idx_t)i * primes[j] <= limit; ++j) {
            spf[(idx_t)i * primes[j]] = primes[j];
        }
    }

    cur_limit = limit;
    primes_cnt = pc;
}

/* ---------- helper: check if rank of m is odd --------------------------- */
static bool rank_is_odd(idx_t m)
{
    idx_t largest_index = 0;
    idx_t parts = 0;

    /* factorise m using the pre‑computed spf array */
    while (m > 1) {
        idx_t p = spf[m];
        idx_t idx = prime_index[p];
        if (idx > largest_index) largest_index = idx;
        ++parts;
        m /= p;
    }
    return ((largest_index - parts) & 1) != 0;
}

/* ---------- external API required by the build ------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* The smallest element to inspect is 2 (the first non‑empty partition). */
    idx_t m = 2;
    idx_t count = 0;

    /* Ensure the sieve is non‑empty before we start factoring. */
    if (cur_limit == 0) build_sieve(16);

    while (count < n) {
        /* Expand the sieve when we reach its current upper bound. */
        if (m > cur_limit) {
            idx_t new_limit = cur_limit ? (cur_limit << 1) : 16;
            while (new_limit < m) new_limit <<= 1;
            build_sieve(new_limit);
        }

        if (rank_is_odd(m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
        ++m;
    }

    /* Should never reach here for valid inputs */
    return 0;
}
