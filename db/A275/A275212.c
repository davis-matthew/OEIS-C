/*
 * A275212  Triangle read by rows, T(n,k) = (n+k+1)! / ([(n-k)/2]! *
 *          [(n+k+2)/2]!) with [.] the floor function, for n>=0 and 0<=k<=n.
 * https://oeis.org/A275212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read row‑by‑row from the triangle T(n,k) = (n+k+1)! / (
 *            floor((n-k)/2)! * floor((n+k+2)/2)! ) where n >= 0 and 0 <= k <=
 *            n. The index n of the flattened sequence lies in row R with n <
 *            (R+1)(R+2)/2 and offset k = n - R(R+1)/2. The value a(n) is donc
 *            T(R,k)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- primes up to 20001, generated once ---------- */
static unsigned int primes[2500];
static size_t prime_cnt = 0;
static bool primes_init = false;

static void init_primes(void)
{
    if (primes_init) return;
    const idx_t LIMIT = 20001U;
    bool *is_comp = (bool *)calloc(sizeof(bool), LIMIT + 1);
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i])
            primes[prime_cnt++] = (unsigned int)i;
    free(is_comp);
    primes_init = true;
}

/* ---------- exponent of prime p in n! ---------- */
static inline int exp_factorial(idx_t n, unsigned int p)
{
    int e = 0;
    while (n) {
        n /= p;
        e += (int)n;
    }
    return e;
}

/* ---------- compute T(n,k) using prime factorisation ---------- */
static seq_t compute_T(idx_t n, idx_t k)
{
    idx_t A = n + k + 1;
    idx_t B = (n >= k) ? (n - k) / 2 : (k - n) / 2;   /* n >= k always */
    idx_t C = (n + k + 2) / 2;

    init_primes();

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt && primes[i] <= A; ++i) {
        unsigned int p = primes[i];
        int e = exp_factorial(A, p) - exp_factorial(B, p) - exp_factorial(C, p);
        if (e > 0) {
            /* compute p^e by binary exponentiation */
            seq_t base = (seq_t)p;
            seq_t p_pow = 1;
            int exp = e;
            while (exp > 0) {
                if (exp & 1)
                    p_pow *= base;
                base *= base;
                exp >>= 1;
            }
            result *= p_pow;
        }
    }
    return result;
}

/* ---------- public sequence generator ---------- */
seq_t a(idx_t n)
{
    /* determine row R and offset k */
    idx_t R = 0;
    while (((R + 1) * (R + 2)) / 2 <= n)
        ++R;
    idx_t prev_total = R * (R + 1) / 2;
    idx_t k = n - prev_total;
    return compute_T(R, k);
}
