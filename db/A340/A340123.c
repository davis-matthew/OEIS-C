/*
 * A340123  Odd composite integers m such that A004254(2*m-J(m,21)) == J(m,21)
 *          (mod m) and gcd(m,21)=1, where J(m,21) is the Jacobi symbol.
 * https://oeis.org/A340123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340123:
   odd composite integers m with gcd(m,21)=1 satisfying
   U(2*m - J(m,21)) ≡ J(m,21) (mod m)
   where U is the Lucas sequence with parameters (a=5,b=1)
   and J(m,21) is the Jacobi symbol (m|21).
   The implementation searches sequentially for such m. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime_big.h"

typedef struct { seq_t a00,a01,a10,a11; } mat2;

static mat2 mul_mat(const mat2 *A, const mat2 *B, seq_t mod)
{
    mat2 C;
    C.a00 = (A->a00 * B->a00 + A->a01 * B->a10) % mod;
    C.a01 = (A->a00 * B->a01 + A->a01 * B->a11) % mod;
    C.a10 = (A->a10 * B->a00 + A->a11 * B->a10) % mod;
    C.a11 = (A->a10 * B->a01 + A->a11 * B->a11) % mod;
    return C;
}

static mat2 mat_pow(mat2 base, seq_t exp, seq_t mod)
{
    mat2 res = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };
    while (exp > 0) {
        if (exp & 1) res = mul_mat(&res, &base, mod);
        base = mul_mat(&base, &base, mod);
        exp >>= 1;
    }
    return res;
}

/* Compute U_n modulo mod, where U_0=0, U_1=1, U_{k+1}=5*U_k - U_{k-1} */
static seq_t lucas_U_mod(seq_t n, seq_t mod)
{
    if (n == 0) return 0;
    mat2 A = { (seq_t)5 % mod, (mod-1)%mod, (seq_t)1 % mod, (seq_t)0 };
    mat2 R = mat_pow(A, n, mod);
    return R.a10 % mod;   /* U_n is the lower-left entry */
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t m = (seq_t)25;   /* first odd composite with gcd(m,21)=1 */
    while (found < n) {
        /* skip numbers divisible by 3 or 7 */
        if ((m % (seq_t)3 == 0) || (m % (seq_t)7 == 0)) {
            m += 2;
            continue;
        }
        /* skip primes */
        if (is_prime_big(m)) {
            m += 2;
            continue;
        }

        int j = jacobi_symbol((uint64_t)m, 21);
        if (j == 0) { m += 2; continue; }   /* should never happen */

        /* k = 2*m - j (j = ±1) */
        seq_t k = ((seq_t)2 * m) + (j == 1 ? (seq_t)(-1) : (seq_t)1);

        seq_t u = lucas_U_mod(k, m);

        seq_t target = (j == 1 ? (seq_t)1 : m - 1); /* -1 mod m */
        if (u == target) {
            found++;
            if (found == n) return m;
        }
        m += 2;   /* next odd candidate */
    }
    return (seq_t)0; /* unreachable */
}
