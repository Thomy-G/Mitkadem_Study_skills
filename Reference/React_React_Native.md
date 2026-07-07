# React & React Native — Study Reference Guide

---

## 1. Core React Declarative Component Design

### 1.1 Declarative UI Rendering
React uses a declarative rendering model where components express what the UI should look like based on the current state. When state changes, React handles updating the browser DOM efficiently.
*   **Single Root Node constraint**: Every JSX/TSX component must return a single root element. Use React Fragments (`<>...</>`) to group sibling elements without adding extra nodes to the DOM tree.

### 1.2 Immutability in State Updates
Never modify state variables directly. Always use setter methods and treat state as read-only.
*   **Correct (Arrays)**: `setItems([...items, newItem])` (creates a new array reference).
*   **Incorrect**: `items.push(newItem); setItems(items);` (keeps the same array reference, preventing React from identifying the change and updating the UI).

### 1.3 Class Components vs. Functional Components
Before hooks (React 16.8), state and lifecycle methods required Class Components. Functional components are now preferred, but you must know how to trace both:

*   **Class Component**: Uses `this.state` in the constructor and `this.setState((prevState) => ...)` to update state. Updates trigger a call to the `render()` method.
*   **Functional Component**: Uses hooks (`useState`, `useEffect`) and renders by re-executing the entire function.

### 1.4 Render Optimization & React.memo
By default, if a Parent component re-renders, all of its Children re-render regardless of whether their props changed.
*   `React.memo` (or `memo` for functional components) wraps a child component and performs a shallow prop comparison. If props haven't changed, it skips rendering the child.
*   For class components, extending `React.PureComponent` performs a similar shallow comparison.

```jsx
import React, { Component, memo } from 'react';

// This child will ONLY render if "value" changes
const MemoizedChild = memo(({ value }) => {
    console.log("Child Rendered");
    return <h3>Child: {value}</h3>;
});

class ParentApp extends Component {
    constructor(props) {
        super(props);
        this.state = { count: 0, unrelated: 0 };
    }

    render() {
        console.log("Parent Rendered");
        return (
            <div>
                <button onClick={() => this.setState({ count: this.state.count + 1 })}>
                    Inc Count
                </button>
                <button onClick={() => this.setState({ unrelated: this.state.unrelated + 1 })}>
                    Inc Unrelated
                </button>
                {/* Child is optimized and won't render if Parent updates 'unrelated' */}
                <MemoizedChild value={this.state.count} />
            </div>
        );
    }
}
```

---

## 2. React Hooks in Detail

### 2.1 `useState`
Manages state values within function components. Updates are asynchronous and batched by React for performance.
```jsx
const [count, setCount] = useState(0);

// If new state depends on previous state, pass a callback to prevent stale closures
const increment = () => {
    setCount(prevCount => prevCount + 1);
};
```

### 2.2 `useEffect`
Enables side-effect execution (data fetching, subscriptions, manual DOM manipulation).
```jsx
useEffect(() => {
    // 1. Setup Phase
    const handleResize = () => console.log(window.innerWidth);
    window.addEventListener('resize', handleResize);
    
    // 2. Cleanup Phase (Crucial to prevent memory leaks and multiple active listeners)
    return () => {
        window.removeEventListener('resize', handleResize);
    };
}, []); // Empty array: runs only once on mount and unmount
```

#### Dependency Array Behaviors:
*   `useEffect(() => {})` (No array): Runs after **every single render** (leads to performance bottlenecks).
*   `useEffect(() => {}, [])` (Empty array): Runs **once** after the initial render.
*   `useEffect(() => {}, [val1, val2])`: Runs on initialization and subsequently **only if** `val1` or `val2` changes.

### 2.3 Custom Hooks
Custom hooks extract component logic into reusable functions. They must begin with the `use` prefix.
```jsx
import { useState, useEffect } from 'react';

function useFetch(url) {
    const [data, setData] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        let active = true;
        setLoading(true);
        fetch(url)
            .then(res => res.json())
            .then(result => {
                if (active) {
                    setData(result);
                    setLoading(false);
                }
            });
        return () => { active = false; };
    }, [url]);

    return { data, loading };
}
```

---

## 3. Component Communication & Context API

### 3.1 Parent-Child Data Sharing
*   **Props Down**: Data passed from parent to child.
*   **Callbacks Up**: Functions passed from parent to child. The child calls the function to send data back up.

### 3.2 Context API
Solves **prop drilling** by providing global state accessible by any descendant component without passing props down manually.

```jsx
import React, { createContext, useContext, useState } from 'react';

const ThemeContext = createContext();

export function ThemeProvider({ children }) {
    const [theme, setTheme] = useState('dark');
    return (
        <ThemeContext.Provider value={{ theme, setTheme }}>
            {children}
        </ThemeContext.Provider>
    );
}

// Child component usage
export function MainPanel() {
    const { theme, setTheme } = useContext(ThemeContext);
    return (
        <div style={{ background: theme === 'dark' ? '#333' : '#fff' }}>
            <button onClick={() => setTheme(theme === 'dark' ? 'light' : 'dark')}>
                Toggle Theme
            </button>
        </div>
    );
}
```

---

## 4. Web React Router (`react-router-dom`)

For web applications requiring navigation, `react-router-dom` is used to configure page navigation, nested routes, and parameters.

### 4.1 Router Architecture & Dynamic Parameters
*   `<BrowserRouter>`: Context wrapper managing URL state history.
*   `<Routes>` & `<Route>`: Maps paths to specific page components.
*   **Dynamic Route Params (`useParams`)**: Extends paths with variables (e.g. `/profile/:username`).
*   **Nested Layouts (`<Outlet>`)**: Parent routes render common layout templates (headers, navbars), using the `<Outlet />` component placeholder to render active subroutes.

```jsx
import React from 'react';
import { BrowserRouter, Routes, Route, Link, useParams, Outlet } from 'react-router-dom';

// Layout screen wrapping subroutes
function Layout() {
    return (
        <div>
            <nav>
                <Link to="/">Home</Link> | <Link to="/about">About</Link> | <Link to="/user/john">Profile</Link>
            </nav>
            <hr />
            {/* Active nested route component is injected here */}
            <Outlet />
        </div>
    );
}

function Home() { return <h2>Home Page</h2>; }
function About() { return <h2>About Page</h2>; }

function UserProfile() {
    // Extract dynamic url param ':username'
    const { username } = useParams();
    return <h2>User Profile: {username}</h2>;
}

function NotFound() { return <h2>404 Page Not Found</h2>; }

export default function AppRouter() {
    return (
        <BrowserRouter>
            <Routes>
                {/* Nested routes inside Layout */}
                <Route path="/" element={<Layout />}>
                    <Route index element={<Home />} />
                    <Route path="about" element={<About />} />
                    <Route path="user/:username" element={<UserProfile />} />
                    <Route path="*" element={<NotFound />} />
                </Route>
            </Routes>
        </BrowserRouter>
    );
}
```

---

## 5. React Native: Cross-Platform Layouts

React Native compiles JS layouts to native Android (XML) and iOS (UIKit) views.

### 5.1 Native Core Components
*   `<View>`: The basic container block. Maps to `div` on web, `android.view.View` on Android, `UIView` on iOS.
*   `<Text>`: Required for all text strings. Unlike HTML, you cannot wrap text in containers without this component.
*   `<ScrollView>`: Simple scroll container. Renders all child items immediately, which can cause performance issues for large datasets.
*   `<FlatList>`: Optimized list view. Only renders elements currently visible on screen, recycling off-screen views.

```jsx
import { FlatList, Text, View, StyleSheet } from 'react-native';

const DATA = [{ id: '1', title: 'First Item' }, { id: '2', title: 'Second Item' }];

export default function ItemList() {
    return (
        <View style={styles.container}>
            <FlatList
                data={DATA}
                keyExtractor={item => item.id}
                renderItem={({ item }) => (
                    <View style={styles.item}>
                        <Text style={styles.title}>{item.title}</Text>
                    </View>
                )}
            />
        </View>
    );
}

const styles = StyleSheet.create({
    container: { flex: 1, paddingTop: 20 },
    item: { padding: 20, borderBottomWidth: 1, borderBottomColor: '#ccc' },
    title: { fontSize: 16 }
});
```

### 5.2 Styling & Flexbox Layouts
*   **Flexbox Defaults**: Flex direction is `column` by default (unlike web CSS, which defaults to `row`).
*   **Measurement units**: Sizes are unitless density-independent pixels (dp). Writing `padding: "10px"` will crash the app.
*   **StyleSheet**: Styles are declared via `StyleSheet.create` for validation and performance optimization.

### 5.3 Navigation Architecture
React Native uses `react-navigation` to manage screens.
*   **Stack Navigator**: Emulates page history by stacking screens on top of each other.
*   **Tab Navigator**: Renders persistent navigation bars at the bottom or top of the screen.

```jsx
import { createNativeStackNavigator } from '@react-navigation/native-stack';

const Stack = createNativeStackNavigator();

function AppNavigation() {
    return (
        <Stack.Navigator initialRouteName="Home">
            <Stack.Screen name="Home" component={HomeScreen} />
            <Stack.Screen name="Details" component={DetailsScreen} />
        </Stack.Navigator>
    );
}

function HomeScreen({ navigation }) {
    return (
        <View>
            <Button title="Go to Details" onPress={() => navigation.navigate('Details')} />
        </View>
    );
}
```

### 5.4 The Native Bridge & Threading Model
React Native runs on three primary threads:
1.  **JS Thread**: Executes all Javascript logic, business code, API calls, and layout configurations.
2.  **UI / Main Thread**: Renders native views and handles user interactions.
3.  **Shadow Thread**: Calculates layout dimensions and positions (using Facebook's Yoga engine) before sending them across the bridge to the UI thread.
*   **The Bridge**: Asynchronously serializes messages into JSON packets to coordinate communication between the JS and Native environments.
