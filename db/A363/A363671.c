/*
 * A363671  Triangular array: row n lists the numbers k such that
 *          prime(n+2)-2*k is a prime.
 * https://oeis.org/A363671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th element (row‑wise) of the triangular array
 * whose row r (1‑based) (excluding the first row) lists all positive
 * integers k such that prime(r+2) − 2·k is prime.
 * That is, a(n) is defined by the enumeration
 *   (1, 1 2, 2 3 4, 1 3 4 5, …)
 * taken row‑wise.  This program computes a(n) by generating primes on
 * demand and then scanning k in each row until the c‑th qualifying
 * value is found.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------- prime cache ------------------------------------- */
static idx_t *prime_cache    = NULL;   /* array of primes */
static size_t prime_count    = 0;      /* number of primes stored */
static size_t prime_capacity = 0;      /* allocated capacity of prime_cache */
static idx_t prime_candidate = 3;      /* current candidate for prime test */

/* Grow prime_cache to at least 'need' primes. */
static void grow_prime_cache(size_t need)
{
    if (need <= prime_capacity) return;
    size_t new_cap = prime_capacity ? prime_capacity << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
    prime_cache   = tmp;
    prime_capacity = new_cap;
}

/* Append a new prime to the cache. */
static void add_prime(idx_t p)
{
    grow_prime_cache(prime_count + 1);
    prime_cache[prime_count++] = p;
}

/* Make sure prime_cache contains at least (idx+1) primes (0‑based index idx).
 * Return the prime at that position. */
static idx_t get_prime_by_index(idx_t idx)
{
    while (prime_count <= idx) {
        if (prime_count == 0) {
            /* prime(0) = 2 */
            add_prime(2);
            prime_candidate = 3;
            continue;
        }
        /* Test current candidate for primality */
        bool is_p = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_cache[i];
            if (( __int128)p * p > prime_candidate) break;
            if (prime_candidate % p == 0) { is_p = false; break; }
        }
        if (is_p) add_prime(prime_candidate);
        prime_candidate += 2;           /* skip even numbers */
    }
    return prime_cache[idx];
}

/* Trial division primality test using cached primes (works for idx_t). */
static bool is_prime_trial(idx_t n)
{
    if (n < 2) return false;
    /* Ensure primes up to sqrt(n) are available. */
    while (prime_count == 0 ||
           (( __int128)prime_cache[prime_count - 1] *
            prime_cache[prime_count - 1] < n)) {
        get_prime_by_index(prime_count);
    }
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_cache[i];
        if (( __int128)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Return the row number r (1‑based) such that the nth element
 * of the flattened array lies in that row. */
static idx_t row_of(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n) high = mid;
        else        low = mid + 1;
    }
    return low;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */

    /* Determine row r and column c within that row. */
    idx_t r = row_of(n);
    __int128 t_prev = (__int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)t_prev;   /* 1‑based column index in row r */

    /* Prime for this row: prime(r+2). Since prime_cache uses 0‑based indexing,
       prime(r+2) is at index r+1. */
    idx_t p = get_prime_by_index(r + 1);

    /* Scan k=1,2,… until the c‑th qualifying k is found. */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* p – 2*k must be ≥ 2 to be prime. */
        if (k > (p - 1) / 2) break;   /* no more candidates */
        idx_t q = p - 2 * k;
        if (is_prime_trial(q)) {
            ++count;
            if (count == c) return (seq_t)k;
        }
    }
    return 0;          /* should never happen for valid n */
}
