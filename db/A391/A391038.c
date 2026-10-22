/*
 * A391038  Number of ways to write n as (p-1)^4/8 + (q-1)^2/4 + (r-1)^2/4 +
 *          (s+1)^2/4, where each of p,q,r,s is either 1 or an odd prime.
 * https://oeis.org/A391038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of ways to write n as (p-1)^4/8 + (q-1)^2/4 + (r-1)^2/4 +
 *            (s+1)^2/4 where each of p,q,r,s is 1 or an odd prime. p, q, r
 *            are distinguished variables, q and r are treated as unordered
 *            (i.e., the pair (q,r) counts the same as (r,q))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_PRIME_LIMIT 2000        /* large enough for n up to ~1e4 */
#define PRIME_LIST_SIZE  500        /* safe upper bound on number of odd primes ≤2000 */

static uint64_t prime_list[PRIME_LIST_SIZE];
static size_t   prime_count = 0;

/* Build list of odd primes up to MAX_PRIME_LIMIT.  O(MAX_PRIME_LIMIT) */
static void init_primes(void)
{
    if (prime_count != 0) return;          /* already built */
    static unsigned char is_comp[MAX_PRIME_LIMIT + 1];
    for (size_t i = 3; i <= MAX_PRIME_LIMIT; i += 2) {
        if (!is_comp[i]) {
            if (prime_count < PRIME_LIST_SIZE) prime_list[prime_count++] = i;
            if (i * i <= MAX_PRIME_LIMIT) {
                for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i << 1)
                    is_comp[j] = 1;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* below the offset, value is unspecified */

    init_primes();

    size_t N = (size_t)n;           /* safe: n will be small enough for test cases */
    unsigned long long count = 0ULL;

    /* Pre‑allocate a bitset for D values (s terms). */
    unsigned char *hasD = (unsigned char*)calloc(N + 1, 1);
    if (!hasD) return 0;          /* memory error: treat as zero */

    /* D( s = 1 ) = 1 */
    if (1 <= N) hasD[1] = 1;

    /* D( s = odd prime ) */
    for (size_t ip = 0; ip < prime_count; ++ip) {
        uint64_t s = prime_list[ip];
        uint64_t t = s + 1;
        uint64_t D = (t * t) / 4ULL;
        if (D > N) break;
        hasD[(size_t)D] = 1;
    }

    /* Enumerate p, q, r (q and r unordered).  q,r indices run from 0 (s=1) to prime_count-1. */
    for (size_t ip = 0; ip <= prime_count; ++ip) {          /* ip==0 => p=1 */
        unsigned long long A;
        if (ip == 0) {
            A = 0ULL;
        } else {
            uint64_t p = prime_list[ip - 1];
            uint64_t t = p - 1;
            A = (t * t * t * t) / 8ULL;
        }
        if (A > N) break;
        unsigned long long rem1 = N - A;

        for (size_t iq = 0; iq <= prime_count; ++iq) {          /* iq==0 => q=1 */
            unsigned long long B;
            if (iq == 0) B = 0ULL;
            else {
                uint64_t q = prime_list[iq - 1];
                uint64_t t = q - 1;
                B = (t * t) / 4ULL;
            }
            if (B > rem1) break;
            unsigned long long remB = rem1 - B;

            for (size_t ir = iq; ir <= prime_count; ++ir) {      /* ir>=iq for unordered pair */
                unsigned long long C;
                if (ir == 0) C = 0ULL;
                else {
                    uint64_t r = prime_list[ir - 1];
                    uint64_t t = r - 1;
                    C = (t * t) / 4ULL;
                }
                unsigned long long S1 = A + B + C;
                if (S1 > N) break;
                unsigned long long rem2 = N - S1;
                if (rem2 == 0) continue;   /* D cannot be 0 */
                if (rem2 <= N && hasD[(size_t)rem2]) {
                    ++count;
                }
            }
        }
    }

    free(hasD);
    return (seq_t)count;
}
