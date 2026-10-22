/*
 * A328571  Primorial base expansion of n converted into its prime product
 *          form, but with all nonzero digits replaced by 1's: a(n) =
 *          A007947(A276086(n)).
 * https://oeis.org/A328571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = radical of the primorial‑base expansion of n. The
 *            primorial‑base of n is expressed as n = Σ d_i · P_i with 0 ≤ d_i
 *            < p_i (p_i = i‑th prime, P_0=1, P_i=∏_{k< i} p_k). a(n) = Π_{i:
 *            d_i≠0} p_i
 * Range:     u8 n <= 217, u16 n <= 9140, u32 n <= 9140, u64 n <= 9140,
 *            u128 n <= 9140, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code is written so that it compiles under C11 and C++17, 
    using only the operations allowed on seq_t.  */

/* Include the standard headers that are needed.  */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */

/* Maximum number of primes that will be needed for the tests.  */
#define MAX_PRIMES 20

/* Static storage for the first MAX_PRIMES primes and the associated
   primorials.  The first primorial (for i=0) is 1.  */
static uint64_t  _primes[MAX_PRIMES];
static seq_t     _primorials[MAX_PRIMES + 1];
static size_t    _prime_cnt    = 0;  /* number of primes currently stored  */
static size_t    _primorial_cnt= 0;  /* index of highest primorial stored +1 */

/* Return true if X is prime, using the primes already stored.  */
static bool
_is_prime(uint64_t X) {
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _primes[i];
        if (p * p > X)
            break;
        if (X % p == 0)
            return false;
    }
    return true;
}

/* Ensure that the arrays contain all primes and primorials with a primorial
   value not exceeding M, where M is given as a seq_t.  */
static void
_ensure_up_to(seq_t M) {
    /* Initialise if the tables are empty.  */
    if (_primorial_cnt == 0) {
        _primorials[0] = (seq_t)1;      /* P_0 = 1  */
        _primorial_cnt = 1;
    }

    /* Generate new primes and primorials until the latest primorial
       exceeds M.  */
    while (_primorials[_primorial_cnt - 1] <= M) {
        /* Generate the next prime if we do not have enough yet.  */
        if (_prime_cnt == 0) {
            _primes[0] = 2;
            _prime_cnt = 1;
        } else {
            uint64_t candidate = _primes[_prime_cnt - 1] + 1;
            while (!_is_prime(candidate))
                ++candidate;
            if (_prime_cnt < MAX_PRIMES) {
                _primes[_prime_cnt++] = candidate;
            } else {
                /* We have run out of space; stop generating.  */
                break;
            }
        }

        /* Compute the next primorial: P_i = P_{i-1} * prime_{i-1}.  */
        if (_primorial_cnt < MAX_PRIMES + 1) {
            _primorials[_primorial_cnt] =
                (seq_t)(_primorials[_primorial_cnt - 1] *
                        (seq_t)_primes[_prime_cnt - 1]);
            ++_primorial_cnt;
        } else {
            break;
        }
    }
}

/* The required sequence function.  */
seq_t
a(idx_t n) {
    /* Convert the index to seq_t once; if seq_t is too small the wrap
       behaviour is defined by the compiler and the sequence is only
       required to be correct where the value actually fits.  */
    seq_t m = (seq_t)n;

    /* Make sure we have primorials large enough for the current n.  */
    _ensure_up_to(m);

    /* Walk over the primorial digits from most significant to least.  */
    seq_t result = (seq_t)1;
    for (size_t i = _primorial_cnt - 1; ; --i) {
        seq_t weight = _primorials[i];
        seq_t digit  = m / weight;              /* 0 <= digit < p_i */
        if (digit != (seq_t)0) {
            /* Multiply by the associated prime.  */
            result *= (seq_t)_primes[i];
        }
        m -= weight * digit;
        if (i == 0)
            break;                              /* all digits processed */
    }

    return result;
}
