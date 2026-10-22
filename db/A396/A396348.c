/*
 * A396348  Array read by ascending antidiagonals: A(n,k) =
 *          prime(n)^A249344(n,k).
 * https://oeis.org/A396348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by reading the array A(i,j) =
 *            prime(i)^{v_{i}(j)} along ascending antidiagonals. For a given
 *            n, let s be the smallest integer with s*(s-1)/2 >= n let i = n -
 *            (s-2)(s-1)/2 let j = s - i then a(n) = prime(i)^{e} where e =
 *            multiplicity of prime(i) in j. If e = 0 the value is 1
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static storage for primes */
static uint32_t *prime_arr = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap > 0 ? prime_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    uint32_t *new_arr = (uint32_t *)realloc(prime_arr, new_cap * sizeof(uint32_t));
    prime_arr = new_arr;
    prime_cap = new_cap;
}

static void generate_primes_up_to(size_t n)
{
    if (prime_cnt >= n)
        return;
    size_t candidate = (prime_cnt == 0) ? 2 : (prime_arr[prime_cnt - 1] + 1);
    if (candidate > 2 && (candidate & 1) == 0)
        candidate++;
    while (prime_cnt < n) {
        uint32_t p = (uint32_t)candidate;
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t pp = prime_arr[i];
            if ((unsigned long long)pp * pp > p)
                break;
            if (p % pp == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            ensure_prime_capacity(prime_cnt + 1);
            prime_arr[prime_cnt++] = p;
        }
        if (p == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}

static uint32_t nth_prime(size_t k)
{
    if (k == 0)
        return 0;
    generate_primes_up_to(k);
    return prime_arr[k - 1];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find diagonal s with s*(s-1)/2 >= n */
    idx_t low = 0, high = 2;
    while (((__int128)high * (high - 1)) / 2 < n)
        high <<= 1;                     /* enlarge search range */
    low = high >> 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid - 1)) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t s = high;                     /* minimal diagonal */

    /* offset within diagonal */
    __int128 prev = ((__int128)(s - 2) * (s - 1)) / 2;
    idx_t k = n - (idx_t)prev;          /* 1 <= k <= s-1 */
    idx_t i = k;                        /* row index (prime index) */
    idx_t j = s - k;                    /* column index (k value) */

    /* prime corresponding to row i */
    uint32_t prime = nth_prime((size_t)i);

    /* exponent of prime in j */
    idx_t tmp = j;
    idx_t e = 0;
    while (tmp % prime == 0) {
        tmp /= prime;
        ++e;
    }

    /* compute prime^e */
    seq_t result = 1;
    for (idx_t t = 0; t < e; ++t)
        result *= (seq_t)prime;

    return result;                      /* 1 if e == 0 */
}
