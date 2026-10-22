/*
 * A356222  Array read by antidiagonals upwards where A(n,k) is the position
 *          of the k-th appearance of 2n in the sequence of prime gaps
 *          A001223. If A001223 does not contain 2n at least k times, set
 *          A(n,k) = -1.
 * https://oeis.org/A356222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value at position n in the array read by
 *            antidiagonals upwards. Each array entry A(i,j) is the position
 *            of the j‑th appearance of the even number 2*i in the prime‐gap
 *            sequence A001223. If the required occurrence does not exist,
 *            a(n) = -1
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Configuration ----------------------------------------------------------- */
#define MAX_GAP   2000   /* maximum even gap to keep (2*i)        */
#define MAX_K     300    /* maximum number of stored occurrences   */
#define SIEVE_LIMIT 20000000ULL /* upper bound for prime generation      */

/* Global data ------------------------------------------------------------ */
static bool initialized = false;
static uint16_t count_gap[MAX_GAP + 1];               /* how many occurrences stored */
static uint32_t pos_gap[MAX_GAP + 1][MAX_K + 1];      /* positions of occurrences   */

/* Utility: integer square root (floor) ---------------------------------- */
static uint64_t floor_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32; /* 2^32 > sqrt(2^64)        */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* Sieve of Eratosthenes -------------------------------------------------- */
static void init(void)
{
    /* Build primes up to SIEVE_LIMIT */
    size_t sieve_n = (size_t)SIEVE_LIMIT + 1;
    uint8_t *sieve = (uint8_t*)calloc(sieve_n, sizeof(uint8_t));
    if (!sieve) exit(EXIT_FAILURE);

    size_t i, p;
    for (p = 2; (uint64_t)p * p <= SIEVE_LIMIT; ++p)
        if (!sieve[p])
            for (i = p * p; i <= SIEVE_LIMIT; i += p)
                sieve[i] = 1;

    /* Count primes */
    size_t prime_count = 0;
    for (i = 2; i <= SIEVE_LIMIT; ++i)
        if (!sieve[i]) ++prime_count;

    /* Store primes in an array */
    uint32_t *primes = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    for (i = 2, prime_count = 0; i <= SIEVE_LIMIT; ++i)
        if (!sieve[i]) primes[prime_count++] = (uint32_t)i;

    free(sieve);

    /* Scan gaps and record positions for even gaps up to MAX_GAP */
    size_t required = (MAX_GAP / 2) * MAX_K;
    size_t collected = 0;
    for (i = 0; i + 1 < prime_count && collected < required; ++i) {
        uint32_t gap = primes[i + 1] - primes[i];
        if (gap <= MAX_GAP && (gap & 1) == 0) {
            uint16_t g = (uint16_t)gap;
            if (count_gap[g] < MAX_K) {
                pos_gap[g][count_gap[g]] = (uint32_t)(i + 1); /* positions are 1‑based */
                ++count_gap[g];
                ++collected;
            }
        }
    }

    free(primes);
    initialized = true;
}

/* Public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)(-1); /* out of offset */

    if (!initialized) init();

    /* Find antidiagonal d such that d*(d+1)/2 >= n */
    uint64_t n_u = (uint64_t)n;
    uint64_t diag = (floor_isqrt(8 * n_u + 1) - 1) / 2;
    uint64_t prev = diag * (diag + 1) / 2;
    if (prev < n_u) {
        ++diag;
        prev = diag * (diag + 1) / 2;
    } else {
        while (diag > 0 && (diag - 1) * diag / 2 >= n_u) {
            --diag;
            prev = diag * (diag + 1) / 2;
        }
    }

    /* Offset within diagonal */
    uint64_t offset = n_u - ((diag - 1) * diag / 2);
    uint64_t zeroIndex = offset - 1;          /* 0‑based index on diagonal */
    uint32_t row = (uint32_t)(diag - zeroIndex); /* i (row) */
    uint32_t col = (uint32_t)(zeroIndex + 1);   /* j (column) */

    /* Access stored position for even number 2*row */
    uint32_t gap_val = 2 * row;
    if (gap_val <= MAX_GAP && col <= count_gap[gap_val])
        return (seq_t)pos_gap[gap_val][col - 1];
    else
        return (seq_t)(-1);
}
