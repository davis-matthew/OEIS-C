/*
 * A320260  Number of ordered pairs (j,k) with 0 < j < k < prime(n)/2 such
 *          that (j*(j+1) mod prime(n)) > (k*(k+1) mod prime(n)).
 * https://oeis.org/A320260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 177, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A320260
    Formula: compute T(p) = #{(j,k): 0<j<k<p/2 , (j(j+1) mod p) > (k(k+1) mod p)}
    where p = prime(n).  Return this value as a seq_t.            */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Fenwick tree (BIT) helpers – 1‑based indexing                     */
/* ------------------------------------------------------------------ */
static inline int fenwick_query(int *bit, int idx)
{
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx)
        sum += bit[idx];
    return sum;
}

static inline void fenwick_update(int *bit, int idx, int size)
{
    for (; idx <= size; idx += idx & -idx)
        bit[idx] += 1;
}

/* ------------------------------------------------------------------ */
/* Prime table – built on first use                                 */
/* ------------------------------------------------------------------ */
#define PRIMES_CAP 12000
#define PRIMES_LIMIT 200000     /* prime(12000)  = 148933 < 200000 */

static uint32_t *prime_list = NULL;   /* pointer to array of primes   */
static size_t    prime_count = 0;     /* number of primes stored      */
static int       primes_initialized = 0;

static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = 1;

    /* Allocate space for primes */
    prime_list = (uint32_t*)malloc(PRIMES_CAP * sizeof(uint32_t));
    if (!prime_list) {
        prime_count = 0;
        return;
    }

    /* Eratosthenes sieve up to PRIMES_LIMIT */
    unsigned char *is_comp = (unsigned char*)malloc(PRIMES_LIMIT + 1);
    if (!is_comp) {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
        return;
    }

    for (unsigned int i = 0; i <= PRIMES_LIMIT; ++i) is_comp[i] = 0;

    size_t cnt = 0;
    for (unsigned int i = 2; i <= PRIMES_LIMIT && cnt < PRIMES_CAP; ++i) {
        if (!is_comp[i]) {
            prime_list[cnt++] = i;
            if ((uint64_t)i * i <= PRIMES_LIMIT) {
                for (unsigned int j = i * i; j <= PRIMES_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    prime_count = cnt;
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/*  Main function (external linkage)                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for offsets < 1 */

    init_primes();
    if (n > prime_count) return 0;        /* out of bounds – should not happen */

    uint32_t p = prime_list[n - 1];
    if (p <= 2) return 0;                 /* for prime = 2 or 3, m = 0 */

    idx_t m = (p - 1) / 2;                /* number of j's: 1 .. m   */

    /* Fenwick tree of size p (1‑based indices) */
    int *bit = (int*)calloc(p + 1, sizeof(int));
    if (!bit) return 0;                   /* allocation failure */

    seq_t inv = 0;                        /* inversion count        */
    idx_t seen = 0;                       /* number of j processed  */

    for (idx_t j = 1; j <= m; ++j) {
        /* compute j*(j+1) mod p using 64‑bit intermediate */
        uint64_t prod = (uint64_t)j * (j + 1);
        uint32_t val = (uint32_t)(prod % p);     /* 0 <= val < p */

        int idx = val + 1;                        /* index in BIT (1‑based) */

        int le = fenwick_query(bit, idx);          /* how many ≤ val seen */
        inv += (seq_t)(seen - le);                 /* those > val */

        fenwick_update(bit, idx, (int)p);
        ++seen;
    }

    free(bit);
    return inv;
}
