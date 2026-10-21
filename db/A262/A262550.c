/*
 * A262550  Read A000010 digit-by-digit.
 * https://oeis.org/A262550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th decimal digit of the concatenation of
    Euler’s totient sequence φ(1) φ(2) φ(3) …  (OEIS A000010) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  Global state used by the function  */
static idx_t   phi_index   = 0;       /* last φ(k) computed (k starts at 1) */
static size_t  digits_used = 0;       /* total digits generated so far  */

static uint8_t *digits      = NULL;  /* buffer holding the digits   */
static size_t  digits_cap   = 0;      /* capacity of the digits buffer */

static uint32_t *primes      = NULL;  /* array of primes found   */
static size_t   primes_cnt   = 0;     /* number of primes stored */
static size_t   primes_cap   = 0;     /* capacity of prime array */

/*  Reallocate a dynamic array of elements of size 'elem_size'   */
static void
grow_array(void   **ptr,
           size_t   *cap,
           size_t    elem_size)
{
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) exit(EXIT_FAILURE);   /* unrecoverable failure */
    *ptr  = tmp;
    *cap  = new_cap;
}

/*  Compute φ(k) for the next k = phi_index+1  */
static void
compute_next_phi(void)
{
    idx_t   k      = phi_index + 1;
    seq_t   result = (seq_t)k;
    seq_t   m      = (seq_t)k;

    /*  factorisation  */
    for (size_t pi = 0;
         pi < primes_cnt &&
             ((uint64_t)primes[pi] * (uint64_t)primes[pi] <= (uint64_t)m);
         ++pi)
    {
        uint32_t p = primes[pi];
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / (seq_t)p;
        }
    }
    if (m > 1)                   /* remaining prime factor > sqrt(k)  */
        result -= result / m;

    /*  Append the digits of 'result' to the global digit buffer  */
    uint8_t  temp[32];
    size_t   len = 0;
    seq_t    x   = result;
    do {                         /* at least one digit is obtained      */
        temp[len++] = (uint8_t)(x % (seq_t)10);
        x /= (seq_t)10;
    } while (x);

    /*  Ensure capacity for the new digits  */
    if (digits_used + len > digits_cap)
        grow_array((void **)&digits, &digits_cap, sizeof(uint8_t));

    /*  Insert digits in the correct order  */
    for (size_t i = 0; i < len; ++i)
        digits[digits_used++] = temp[len - 1 - i];

    /*  Detect whether k is a prime to extend the prime list  */
    bool is_prime = true;
    for (size_t pi = 0;
         pi < primes_cnt &&
             ((uint64_t)primes[pi] * (uint64_t)primes[pi] <= (uint64_t)k);
         ++pi)
        if (k % primes[pi] == 0) { is_prime = false; break; }

    if (k >= 2 && is_prime) {               /* avoid treating 1 as prime   */
        if (primes_cnt == primes_cap)
            grow_array((void **)&primes, &primes_cap, sizeof(uint32_t));
        primes[primes_cnt++] = (uint32_t)k;
    }

    ++phi_index;
}

/*  Ensure the digit buffer contains at least 'need' digits.  */
static void
ensure_digits(idx_t need)
{
    while (digits_used < need) {
        /*  Initialise the prime list with 2 at the first call  */
        if (primes_cnt == 0) {
            if (primes_cap == 0)
                grow_array((void **)&primes, &primes_cap, sizeof(uint32_t));
            primes[primes_cnt++] = 2;
        }
        compute_next_phi();
    }
}

/*  Public function required by the OEIS test harness  */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return safe 0 */

    ensure_digits(n);
    return (seq_t)digits[(size_t)(n - 1)];
}
