import numpy as np


def forward(A, B, pi, O):
    """
    :param A: state transition matrix (NxN)
    :param B: observation probability matrix (NxM)
    :param pi: initial state probabilities (N)
    :param O: observation sequence (T)

            2 <= N <= 10
            2 <= M <= 10
            1 <= T <= 30

            N is the number of states
            M is the number of possible observations
            T is the length of the observation sequence

            A[i][j] is the transition probability from state i to state j
            B[i][j] is the probability of observing observation j in state i
            pi[i] is the probability of initial state is being state i
            O[k] is the k-th observation, which is an index between 0 and M-1 (inclusive)

    :return: given the model(A, B, pi), probability of the observation sequence
    """
    N = A.shape[0]
    T = O.shape[0]

    alpha = np.zeros((T, N))
    first_obs = O[0]
    alpha[0] = pi * B[:, first_obs]

    for i in range(1, T):
        alpha[i] = alpha[i-1].dot(A) * B[:,O[i]]

    summ = sum(alpha[-1])

    return summ


def viterbi(A, B, pi, O):
    """
    :param A: state transition matrix (NxN)
    :param B: observation probability matrix (NxM)
    :param pi: initial state probabilities (N)
    :param O: observation sequence (T)

            2 <= N <= 10
            2 <= M <= 10
            1 <= T <= 30

            N is the number of states
            M is the number of possible observations
            T is the length of the observation sequence

            A[i][j] is the transition probability from state i to state j
            B[i][j] is the probability of observing observation j in state i
            pi[i] is the probability of initial state is being state i
            O[k] is the k-th observation, which is an index between 0 and M-1 (inclusive)

    :return: given the model(A, B, pi) and observation sequence O, the most likely state sequence.
             It should be a NumPy array with size T. It includes state indices according to A's indices.
             For example: [1, 2, 1, 1, 0, 4]
    """
    np.seterr(divide="ignore")

    N = A.shape[0]
    T = O.shape[0]

    omega = np.zeros((T, N))
    first_obs = O[0]
    omega[0] = np.log(pi * B[:, first_obs])

    previous = np.zeros((T-1, N))

    for i in range(T-1):
        prob = omega[i] + np.log(A.transpose())
        for j in range(N):
            prob[j] += np.log(B[j, O[i+1]])
        previous[i] = np.argmax(prob,axis=1)
        omega[i+1] = np.amax(prob, axis=1)

    last_state = np.argmax(omega[-1])

    state_seq = np.zeros(T)
    state_seq[0] = last_state

    for i in range(T-1):
        col = int(last_state)
        state_seq[i+1] = previous[T-2-i, col]
        last_state = previous[T-2-i, col]

    state_seq = state_seq[::-1]

    return state_seq